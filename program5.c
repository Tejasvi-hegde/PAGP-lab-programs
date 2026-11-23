// Program 5: MPI message passing with 4 processes.
// Ranks 1, 2, 3 send HELLO, CSE, RVCE to rank 0.
// Compile: mpicc program5_mpi_message.c -o p5
// Run:     mpirun -np 4 ./p5
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define BUFFER_SIZE 32

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (size != 4) {
		if (rank == 0) printf("Run with: mpirun -np 4 ./p5\n");
		MPI_Finalize();
		return 1;
	}

	char *messages[4] = { "", "HELLO", "CSE", "RVCE" };
	char buf[BUFFER_SIZE];

	if (rank != 0) {
		strcpy(buf, messages[rank]);
		MPI_Send(buf, BUFFER_SIZE, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	} else {
		for (int i = 1; i < size; i++) {
			MPI_Recv(buf, BUFFER_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("Process 0 received \"%s\" from process %d\n", buf, i);
		}
	}

	MPI_Finalize();
	return 0;
}
