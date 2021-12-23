#include <iostream>
#include <mpi.h>

int main(int argc, char **argv) {
    int size, rank, position, i;
    float a[10];
    char b[10], buf[100];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::cout << '\n';
    position = 0;
    for (i = 0; i < 10; i++) {
        a[i] = rank + 1.0;
        if (rank == 0) b[i] = 'a';
        else b[i] = 'b';
    }
    //printing values of a and b before packing and broadcasting
    std::cout << "before: process rank = " << rank << '\n';
    for (i = 0; i < 10; i++) {
        std::cout << "a[" << i << "] = " << a[i] << "\t" << "b[" << i << "] = " << b[i] << '\n';
    }
    std::cout << '\n';
    if (rank == 0) {
        //packing a and b to the buffer accordint to datatype
        MPI_Pack(a, 10, MPI_FLOAT, buf, 100, &position, MPI_COMM_WORLD);
        MPI_Pack(b, 10, MPI_CHAR, buf, 100, &position, MPI_COMM_WORLD);
        MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
    } else {
        MPI_Bcast(buf, 100, MPI_PACKED, 0, MPI_COMM_WORLD);
        position = 0;
        //unpacking
        MPI_Unpack(buf, 100, &position, a, 10, MPI_FLOAT, MPI_COMM_WORLD);
        MPI_Unpack(buf, 100, &position, b, 10, MPI_CHAR, MPI_COMM_WORLD);
    }
    //printing values of a and b after unpacking
    std::cout << "after: process rank = " << rank << '\n';
    for (i = 0; i < 10; i++) {
        std::cout << "a[" << i << "] = " << a[i] << "\t" << "b[" << i << "] = " << b[i] << '\n';
    }
    std::cout << '\n';
    MPI_Finalize();
}