#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <mpi.h>

int ROOT = 0;

//summing up int array elements
int sumArray(int *x, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum = sum + x[i];

    return sum;
}

//parallel sum using MPI_Reduce method with MPI_SUM as OP
int sumReduce(int x[], int batchSize) {
    int localSum = 0;
    int totalSum = 0;

    localSum = sumArray(x, batchSize);
    //sending local sum to root and reducing in total sum via MPI_SUM
    MPI_Reduce(&localSum, &totalSum, 1, MPI_INT, MPI_SUM, ROOT, MPI_COMM_WORLD);

    return totalSum;
}

//parallel sum with MPI_SUM as a double operation
int sumDoubling(int x[], int batchSize, int rank, int n, MPI_Status status) {
    int totalSum, subsumReceiver, r, child;
    totalSum = sumArray(x, batchSize);

    //cascade doubling scheme implementation
    //NB: n is required to be power of two, since there is a structure like a binary tree
    for (int p = 2; p <= n; p *= 2) {
        r = rank % p;
        child = rank + p / 2;
        if (r == 0 & child < n) {
            MPI_Recv(&subsumReceiver, 1, MPI_INT, child, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            totalSum += subsumReceiver;
        } else {
            MPI_Send(&totalSum, 1, MPI_INT, rank - r, ROOT, MPI_COMM_WORLD);
        }
    }

    return totalSum;
}

int main(int argc, char *argv[]) {
    //reading array size
    int arrayLength = atoi(argv[1]);
    //reading key distinguishing which method (reduce (-r option) or cascade doubling (no param)) use in this run
    std::string methodKey;
    if (argc > 2) {
        std::string methodKey = argv[2];
    }

    int *arr;
    int rank, n, batchSize, totalSum;
    double startTime, endTime;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    //size of array chunk per process (in assumption of [arrayLength / n] is int)
    batchSize = arrayLength / n;

    if (rank == ROOT) {
        //input array to sum up initialization in the root process
        arr = new int[arrayLength]();
        for (int i = 0; i < arrayLength; i++) arr[i] = 1;
    }

    //buffer array to
    int *batch = new int[batchSize]();

    //starting time measurements in the root process
    if (rank == ROOT)
        startTime = MPI_Wtime();

    //sending chunks of arr a with size = batchSize from ROOT process to others
    MPI_Scatter(arr, batchSize, MPI_INT, batch, batchSize, MPI_INT, ROOT, MPI_COMM_WORLD);

    //choosing method according to second arg
    if (methodKey == "-r") {
        totalSum = sumReduce(batch, batchSize);
    } else {
        totalSum = sumDoubling(batch, batchSize, rank, n, status);
    }

    if (rank == ROOT) {
        //time measurements in the root process again and printing info
        endTime = MPI_Wtime();
        printf("Process rank = %d\n", rank);
        printf("Sum =  %d\n", totalSum);
        printf("Run time = %f\n", endTime - startTime);
    }

    MPI_Finalize();
}
