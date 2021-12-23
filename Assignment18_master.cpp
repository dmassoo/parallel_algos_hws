#include <iostream>
#include <mpi.h>
int main(int argc, char **argv)
{
	int size, rank1, rank2, n_ps;
	MPI_Status status;
	//intercommunicator to communicate between groups
	MPI_Comm intercomm;
	char slave[200] = "/mnt/c/Users/dimas/CLionProjects/HW/parallel_algos_hws/slave.o";
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//spawns 3 new processes with program name 'slave' with root 0
	MPI_Comm_spawn(slave, MPI_ARGV_NULL, 3, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);
	//receive from first two slaves their ranks
	MPI_Recv(&rank1, 1, MPI_INT, 0, 0, intercomm, &status);
	MPI_Recv(&rank2, 1, MPI_INT, 1, 1, intercomm, &status);
	//Display "Slaves rank1 and rank2 are working", instead of the words rank1 and rank2 their values should be displayed.
	std::cout << "Slaves " << rank1 << " and " <<  rank2 << " are working!" << '\n';
	//receive number of slave processes from third slave
	MPI_Recv(&n_ps, 1, MPI_INT, 2, 2, intercomm, &status);
    std::cout << "Number of slaves = " << n_ps << '\n';
	MPI_Finalize();
	return 0;
}