#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	int rank, n, i, message;
    int SIXTH = 6;
    double start_t, fin_t;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//    my version is 6, so, sixth process receives the messages
    start_t = MPI_Wtime();
	if (rank == SIXTH)
	{
        cout << "Hello from process " << rank << '\n';
		for (i = 1; i < n; i++) {
			MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			cout << message << '\n';
		}
	}
//	other processes sends their ranks to p with rank = 6
	else MPI_Send(&rank, 1, MPI_INT, SIXTH, 0, MPI_COMM_WORLD);
	fin_t = MPI_Wtime();
	cout << "Execution time for the process " << rank << "= " << (fin_t - start_t) << '\n';
	MPI_Finalize();
	return 0;
}