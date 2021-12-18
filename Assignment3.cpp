#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char *argv[]) {
    //starting parallel
    MPI_Init(&argc, &argv);
    int rank, n, i, message;
    MPI_Status status;
    // number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    // process id (called rank in MPI ecosystem)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // process with p_id = 0 receives messages from the rest
    if (rank == 0) {
        cout << "Hello from process " << rank << "\n";
        for (i = 1; i < n; i++) {
            MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            cout << "Hello from process " << message << endl;
        }
    // rank != 0: sends its rank to p with rank = 0
    } else MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    // finalization of parallel code
    MPI_Finalize();
    return 0;
}