#include <iostream>
#include <mpi.h>
#define NTIMES 100
using namespace std;
int main(int argc, char **argv)
{
	double time_start, time_finish;
	int rank, i;
	int len;
	char *name = new char;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// processor name: name is a unique name of a actual node, len is length of the name
	MPI_Get_processor_name(name, &len);
	// fix time
	time_start = MPI_Wtime();
	// 1
	// fix time for each iteration
	for (i = 0; i < NTIMES; i++)
		time_finish = MPI_Wtime();
	// 2
	// find process exec time between 1 and 2 and print  average iteration time in std out
	cout << "processor " << name << ", process " << rank << "time = " << (time_finish - time_start) / NTIMES << endl;
	MPI_Finalize();
}