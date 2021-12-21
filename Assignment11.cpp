#include <iostream>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, size, prev, next;
    int buf[2];
    MPI_Init(&argc, &argv);
    MPI_Request reqs[4];
    MPI_Status stats[4];
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // setting ranks of the prev and the next processes
    prev = rank - 1;
    next = rank + 1;
    // prev for 0 is the last one (creating such a ring)
    if (rank == 0) prev = size - 1;
    // next for the last process is the first one (ring in the other order)
    if (rank == size - 1) next = 0;

    // sending and receiving from prev and next asynchronously
    // sendrecv combines send and receive
    MPI_Sendrecv(&rank, 1, MPI_INT, next, 0, &buf[0], 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &stats[0]);
    MPI_Sendrecv(&rank, 1, MPI_INT, prev, 0, &buf[1], 1, MPI_INT, next, 0, MPI_COMM_WORLD, &stats[1]);

    //Your code here.
    //Here you need to display the number of the current process, and what it receives from the previous and next processes.
    std::cout << "Process number = " << rank << ". Received from previous: " << buf[0] << ". Received from next: "
              << buf[1] << '\n';
    MPI_Finalize();
}












