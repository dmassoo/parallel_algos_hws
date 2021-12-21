#include <iostream>
#include <mpi.h>

int main(int argc, char **argv) {
    // boilerplate almost like as in the several previous tasks
    int rank, size, prev, next;
    int rbuf[2], sbuf[2];
    MPI_Init(&argc, &argv);
    MPI_Status stats[4];
    MPI_Request reqs[4];
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    prev = rank - 1;
    next = rank + 1;
    if (rank == 0) prev = size - 1;
    if (rank == size - 1) next = 0;

    // creating persistent requests for a receive
    MPI_Recv_init(&rbuf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
    MPI_Recv_init(&rbuf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]);
    // creating persistent requests for a send
    MPI_Send_init(&sbuf[0], 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
    MPI_Send_init(&sbuf[1], 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);

    // init send buffer with current process rank
    sbuf[0] = rank;
    sbuf[1] = rank;
    // for each request in reqs start it
    for (int i = 0; i < 4; i++) {
        MPI_Start(&reqs[i]);
    }
    // wait all sends and receives in the reqs and saving statuses in stats
    MPI_Waitall(4, reqs, stats);

    // print the same as in the prev tasks about the ring topology
    std::cout << "Process number = " << rank << ". Received from previous: " << rbuf[0] << ". Received from next: "
              << rbuf[1] << '\n';
}
