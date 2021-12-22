#include <iostream>
#include <mpi.h>
#define n 1000

void max(void *a, void *b, int *l, MPI_Datatype *type) {
    int* in = (int*) a;
    int* out = (int*) b;
    for (int i = 0; i < *l; i++)
        if (in[i] > out[i]) out[i] = in[i];
}

int main(int argc, char **argv)
{
	int rank, size, i;
	int a[n];
	int b[n], b_t[n];
	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (i = 0; i < n; i++) a[i] = i + rank + 1;
	printf("process %d a[0] = %d\n", rank, a[0]);
	//creating custom operation with our max fun
	MPI_Op_create(&max, 1, &op);
	//reduce using this OP
	MPI_Reduce(a, b, n, MPI_INT, op , 0, MPI_COMM_WORLD);
	MPI_Op_free(&op);
	//reduce using built-in MPI_MAX OP
    MPI_Reduce(a, b_t, n, MPI_INT, MPI_MAX , 0, MPI_COMM_WORLD);
    // checking results in process 0
	if (rank == 0) {
        printf("b[0] = %d\n", b[0]);
        printf("b_check[0] = %d\n", b[0]);

	    bool isCorrect = true;
        for (int i = 0; i < n; i++) {
            if (b[i] != b_t[i]) {
                isCorrect = false;
                std::cout << "Results not the same, check your OP" << std::endl;
                break;
            }
        }
        if (isCorrect) std::cout << "Correct results" << std::endl;
    }
	MPI_Finalize();
}
