#include <stdio.h>
#include <mpi.h>
#include <string.h>
#define BUFSIZE 100

void create_file(const char* file_name, const char* content) {
    MPI_File f;
    MPI_Status status;
    MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &f);
    MPI_File_write(f, content, strlen(content), MPI_CHAR, &status);
    MPI_File_close(&f);
}

int main(int argc, char **argv) {
	int bufsize, num, sum;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_File fh;
	char buf[BUFSIZE];
	//file creation
	create_file("file.txt", "Some arbitrary text...");
	//file open
	MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	//setting view
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	sum = 0;

	do {
        //reading file chunks into the buffer
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
		//and counting the number of the elements read from the status
		MPI_Get_count(&status, MPI_CHAR, &num);
		printf("buf=%s\n", buf);
		sum += num;
	} while (num >= BUFSIZE);
	MPI_File_close(&fh);
	//print the number of read symbols sum from the file
    printf("Number of read symbols sum from the file: %d.\n", sum);
    //logic of deletion according to commandline key '-d'
    if (argc > 1 && strcmp(argv[1], "-d") == 0) {
        MPI_File_delete("file.txt", MPI_INFO_NULL);
    }
	MPI_Finalize();
}