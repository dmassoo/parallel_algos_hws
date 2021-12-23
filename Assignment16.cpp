#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
	int rank, size, rank1, d = 3;
	MPI_Init(&argc, &argv);
	MPI_Comm comm_revs;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) std::cout << "Split key is: rank % " << d << '\n';
    //splitting to the disjoint subgroups according the second parameter called color
    //new rank is assigned via 3rd parameter
	MPI_Comm_split(MPI_COMM_WORLD, rank % d, size - rank, &comm_revs);
	//write process rank in the new communicator to rank1
	MPI_Comm_rank(comm_revs, &rank1);
	//Display rank and rank1
	std::cout << "rank = " << rank << "\trank1 = "<< rank1 << '\n';
	MPI_Comm_free(&comm_revs);
	MPI_Finalize();
}