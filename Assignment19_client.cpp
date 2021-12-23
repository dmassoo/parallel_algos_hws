#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int rank, r, m = 42;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    char port_name[MPI_MAX_PORT_NAME];
    MPI_Comm intercomm;
    //Copies first cmd line arg to port_name
    strcpy(port_name, argv[1]);

    printf("Attempt to connect\n");
    //Make a request to form a new intercommunicator
    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    printf("Server connection\n");
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //Sends msg to the server
    MPI_Send(&m, 1, MPI_INT, 0, 0, intercomm);
    //Receives msg from the server
    MPI_Recv(&r, 1, MPI_INT, 0, 0, intercomm, &status);
    printf("Client sent value:%d\n", m);
    printf("Client got value:%d\n", r);
    MPI_Finalize();
    return 0;
}