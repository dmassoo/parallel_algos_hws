#include <iostream>
#include <mpi.h>

void init_arr(int *a, int size) {
    for (int i = 0; i < size; i++) {
        a[i] = 1;
    }
    return;
}

int main(int argc, char **argv) {
    int MEGA = 1024 * 1024;
    int rank, size, n = 10000, n_tr = 10;
    int empty_msg;
    double start_time, finish_time, latency, tr;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int max_size = 1000000;
    int a[max_size];
    init_arr(a, size);
    int msg_size = 10;
    int L;
    ////this code is about bandwidth
    if (rank == 0) {
        while (true) {
            start_time = MPI_Wtime();
            for (int i = 1; i <= n_tr; i++) {
                MPI_Send(&a[0], msg_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&a[0], msg_size, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            }
            finish_time = MPI_Wtime();
            L = sizeof(int) * msg_size;
            tr = 2 * n_tr * L / (MEGA * (finish_time - start_time));
            msg_size *= 10;
            std::cout << tr << '\n' << '\n';
        }
    } else {
        while (true) {
            for (int i = 1; i <= n_tr; i++) {
                MPI_Recv(&a[0], msg_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(&a[0], msg_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
            msg_size *= 10;
        }
    }
    return 0;


    ////this code is about latency measurements
//    if (rank == 0) {
//        start_time = MPI_Wtime();
//        for (int i = 1; i <= n; i++) {
//            MPI_Send(&empty_msg, 0, MPI_INT, 1, 0, MPI_COMM_WORLD);
//            MPI_Recv(&empty_msg, 0, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
//        }
//        finish_time = MPI_Wtime();
//        latency = (finish_time - start_time) / n;
//        std::cout << latency << '\n';
//    } else {
//        for (int i = 1; i <= n; i++) {
//            MPI_Recv(&empty_msg, 0, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
//            MPI_Send(&empty_msg, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
//        }
//    }
//    return 0;
}
