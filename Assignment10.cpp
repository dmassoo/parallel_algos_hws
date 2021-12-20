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
    // receiving from prev and next and sending async messages to prev and next
    // it's important to note the tags: receiving from prev with 5 and sending to the next is 5, which is consistent
    // tag 6 is also used for for correct messages addressing in the ring
    MPI_Irecv(&buf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
    MPI_Irecv(&buf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]);
    MPI_Isend(&rank, 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
    MPI_Isend(&rank, 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);
    MPI_Waitall(4, reqs, stats);

    //Your code here.
    //Here you need to display the number of the current process, and what it receives from the previous and next processes.
    std::cout << "Process number = " << rank << ". Received from previous: " << buf[0] << ". Received from next: "
              << buf[1] << '\n';
    MPI_Finalize();
}