#include <iostream>
#include <mpi.h>


int main(int argc, char **argv) {
    int rank, size, n = 10000;
    int empty_msg;
    double start_time, finish_time, latency;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    ////this code is about latency measurements
    if (rank == 0) {
        start_time = MPI_Wtime();
        for (int i = 1; i <= n; i++) {
            MPI_Send(&empty_msg, 0, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&empty_msg, 0, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        }
        finish_time = MPI_Wtime();
        latency = (finish_time - start_time) / n;
        std::cout << latency << '\n';
    } else {
        for (int i = 1; i <= n; i++) {
            MPI_Recv(&empty_msg, 0, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&empty_msg, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    return 0;
}
