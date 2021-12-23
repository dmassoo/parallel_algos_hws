#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int r, m = 25;
    MPI_Init(&argc, &argv);
    char port_name[MPI_MAX_PORT_NAME];
    MPI_Status status;
    MPI_Comm intercomm;
    //Establishes an address that can be used
    //to establish connections between groups of MPI processes.
    MPI_Open_port(MPI_INFO_NULL, port_name);
    printf("Port name: %s\n", port_name);
    //Accept a request to form a new intercommunicator (last arg)
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    printf("Client connected\n");
    //Receive msg from the client
    MPI_Recv(&r, 1, MPI_INT, 0, 0, intercomm, &status);
    //Send msg to the client
    MPI_Send(&m, 1, MPI_INT, 0, 0, intercomm);
    printf("Server sent value: %d\n", m);
    printf("Server got value: %d\n", r);
    //Frees communicator
    MPI_Comm_free(&intercomm);
    //Close port.
    MPI_Close_port(port_name);
    MPI_Finalize();
    return 0;
}