#include <iostream>
#include <mpi.h>

using namespace std;
void init_arr(int *a, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = 1;
    }
    return;
}

// NB: possible overflow due to int type (but we use in with I arrays, so it will be fine)
int dot_prod(int *a, int *b, int size) {
    long int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

int main(int argc, char **argv)
{
    int ARR_SIZE = 1000000;
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int n_workers = size - 1;
    int segment_size = ARR_SIZE / n_workers;

    if (rank == 0) {
        int a[ARR_SIZE], b[ARR_SIZE];
        init_arr(a, ARR_SIZE);
        init_arr(b, ARR_SIZE);

        for (int i = 1; i <= n_workers; i++) {
            // 1) split arrays and send to according processes
            // NB: in assumption that ARR_SIZE / n_workers is integer (ie number  processes total in (5, 6, 9, 11,...)
            MPI_Send(&a[(i - 1) * segment_size], segment_size, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&b[(i - 1) * segment_size], segment_size, MPI_INT, i , 2, MPI_COMM_WORLD);
        }
        // 3) receive subsums and reduce them
        int result = 0;
        int subs = 0;
        for (int i = 1; i <= n_workers; i++) {
            MPI_Recv(&subs, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, &status);
            result += subs;
        }
        cout << "Dot product = " << result << '\n';
    }
    else {
        // 2) receive messages, calc dot product subarrs and send back
        int a_i[segment_size];
        int b_i[segment_size];
        MPI_Recv(&a_i[0], segment_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&b_i[0], segment_size, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        int sum = dot_prod(a_i, b_i, segment_size);
        MPI_Send(&sum, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
