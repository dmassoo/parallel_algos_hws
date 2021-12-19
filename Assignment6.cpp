#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char **argv)
{
	int rank, size, ibuf;
	MPI_Status status;
	float rbuf;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	ibuf = rank;
	rbuf = 1.0 * rank;
	// process with rank 1 sends its rank to process 0 with tag 5
	if (rank == 1) MPI_Send(&ibuf, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    // process with rank 2 sends its rank (but in the float type) to process 0 with tag 5
	if (rank == 2) MPI_Send(&rbuf, 1, MPI_FLOAT, 0, 2, MPI_COMM_WORLD);
	if (rank == 0) {
	    // checking for the messages from any process and saving status in the MPI_Status structure
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		// if message from the process 1, receive both messages (from 1, then 2) and print info
		if (status.MPI_TAG == 1) {
			MPI_Recv(&ibuf, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
			MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 2, MPI_COMM_WORLD, &status);
			cout << "Process 0 recv " << ibuf << " from process 1, " << rbuf << " from process 2\n";
		}
		// if message from the process 2, receive both messages (from 2, then 1) and print info
		else if (status.MPI_TAG == 2) {
			MPI_Recv(&rbuf, 1, MPI_FLOAT, 2, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&ibuf, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
			cout << "Process 0 recv " << rbuf << " from process 2, " << ibuf << " from process 1\n";
		}
	}
	MPI_Finalize();
}