#include <iostream>
#include <mpi.h>

// The function of allocating memory for a two-dimensional array
double **malloc_array(long int n) {
    double **matrix = new double *[n];
    for (int i = 0; i < n; i++)
        matrix[i] = new double[n];
    return matrix;
}

// Memory free function
void free_array(double **matrix, long int n) {
    for (int i = 0; i < n; i++)
        delete[] matrix[i];
    delete[] matrix;
}

// Matrix zeroing function
void zero_init_matrix(double **matrix, long int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = 0.0;
}

void just_init_matrix(double **matrix, long int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = i + j;
}



int main(int argc, char **argv) {
    int size, rank, N = 500;
    double start_time, finish_time;
    double **A, **B, **C;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Memory allocation for matrices A, B, C
    A = malloc_array(N);
    B = malloc_array(N);
    C = malloc_array(N);

    // Initialization of matrices
    just_init_matrix(A, N);
    just_init_matrix(B, N);
    zero_init_matrix(C, N);

    //barrier process synchronization
	MPI_Barrier(MPI_COMM_WORLD);
    // start timing for each process
    start_time = MPI_Wtime();
    // matrix multiplication
    for (int j = 0; j < N; j++) {
        for (int k = 0; k < N; k++) {
            for (int i = 0; i < N; i++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    // end of timing
    finish_time = MPI_Wtime() - start_time;
    // output the execution time of matrix multiplication at each process
    std::cout << "Time in seconds for process " << rank << " = " << finish_time << '\n';
	MPI_Finalize();
}