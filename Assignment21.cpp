#include <stdio.h>
#include <mpi.h>
int main(int argc, char **argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_File fh;
	int text_size = 15;
	char buf[text_size];
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	//opens file file.txt from current directory
	MPI_File_open(MPI_COMM_WORLD, "file1.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	//sets the file view
	// rank * text_size displacement shift the part of file to read with rank increasing
	//and the read chunk size = text_size
	MPI_File_set_view(fh, rank * text_size, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    //collective read using individual file pointer into the buffer.
	MPI_File_read_all(fh, buf, text_size, MPI_CHAR, MPI_STATUS_IGNORE);
	//print the buffer content and the rank
	printf("process %d, buf=%s\n", rank, buf);
	//closes file
	MPI_File_close(&fh);
	MPI_Finalize();
}