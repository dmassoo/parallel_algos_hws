//// example from the lecture was taken as a baseline solution


#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

using namespace std;

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

// The function of initializing a matrix with random numbers from [0,1]
void rand_init_matrix(double **matrix, long int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = rand() / RAND_MAX;
}

// Matrix zeroing function
void zero_init_matrix(double **matrix, long int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = 0.0;
}

void just_init_matrix(double **matrix, long int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = i + j;
}

int main(int argc, char *argv[]) {
    const long int N = atoll(argv[1]);
    double **A, **B, **C;

// Memory allocation for matrices A, B, C
    A = malloc_array(N);
    B = malloc_array(N);
    C = malloc_array(N);

// Initialization of matrices
//    rand_init_matrix(A, N);
//    rand_init_matrix(B, N);
    zero_init_matrix(C, N);
    clock_t t;

////    TEST CODE: uncomment to init
    just_init_matrix(A, N);
    just_init_matrix(B, N);


// Matrix multiplication with cycle order ijk
    double times[10];
    int i, j, k;
    for (int nthr = 1; nthr <= 10; ++nthr) {
        std::cout << "Number of threads = " << nthr << std::endl;
        t = clock();
        #pragma omp parallel for num_threads(nthr) shared(A, B, C) private(i, j, k)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    C[i][j] = A[i][k] * B[k][j];
        }
        t = clock() - t;
        cout << "Time ijk loops is " << t / CLOCKS_PER_SEC << " seconds" << endl;
        times[nthr - 1] = t;
//        cout << "Result:\n";
//        for (int i = 0; i < N; i++) {
//            for (int j = 0; j < N; j++) {
//                cout << C[i][j] << " ";
//            }
//            cout << endl;
//        }
    }

    for (int i = 0; i < 10; i++) {
        cout << "Efficiency for " << i + 1 << " threads = " << times[0] / times[i] << '\n';
    }
    for (int i = 0; i < 10; i++) {
        cout << times[0] / times[i] << '\n';
    }
    cout << '\n';

// Matrix multiplication with cycle order jki
    for (int nthr = 1; nthr <= 10; ++nthr) {
        std::cout << "Number of threads = " << nthr << std::endl;
        zero_init_matrix(C, N);
        t = clock();
        #pragma omp parallel for num_threads(nthr) shared(A, B, C) private(i, j, k)
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++)
                for (int i = 0; i < N; i++)
                    C[i][j] = A[i][k] * B[k][j];
        }
        t = clock() - t;
        cout << "Time jki loops is " << t / CLOCKS_PER_SEC << " seconds" << endl;
        cout << endl;
        times[nthr - 1] = t;
//        cout << "Result:\n";
//        for (int i = 0; i < N; i++) {
//            for (int j = 0; j < N; j++) {
//                cout << C[i][j] << " ";
//            }
//            cout << endl;
//        }
    }
    for (int i = 0; i < 10; i++) {
        cout << "Efficiency for " << i + 1 << " threads = " << times[0] / times[i] << '\n';
    }
    for (int i = 0; i < 10; i++) {
        cout << times[0] / times[i] << '\n';
    }
    cout << '\n';

// Matrix multiplication with cycle order ikj
    for (int nthr = 1; nthr <= 10; ++nthr) {
        std::cout << "Number of threads = " << nthr << std::endl;
        zero_init_matrix(C, N);
        t = clock();
        #pragma omp parallel for num_threads(nthr) shared(A, B, C) private(i, j, k)
        for (i = 0; i < N; i++) {
            for (k = 0; k < N; k++)
                for (j = 0; j < N; j++)
                    C[i][j] = A[i][k] * B[k][j];
        }
        t = clock() - t;
        cout << "Time ikj loops is " << t / CLOCKS_PER_SEC << " seconds" << endl;
        times[nthr - 1] = t;

//        cout << "Result:\n";
//        for (int i = 0; i < N; i++) {
//            for (int j = 0; j < N; j++) {
//                cout << C[i][j] << " ";
//            }
//            cout << endl;
//        }
    }
    for (int i = 0; i < 10; i++) {
        cout << "Efficiency for " << i + 1 << " threads = " << times[0] / times[i] << '\n';
    }
    for (int i = 0; i < 10; i++) {
        cout << times[0] / times[i] << '\n';
    }
// Freeing memory occupied by matrices A, B, C
    free_array(A, N);
    free_array(B, N);
    free_array(C, N);

    return 0;
}
