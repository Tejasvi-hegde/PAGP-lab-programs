// Program 1b: Estimate PI using Monte Carlo (MPI)
// Compile: mpic++ program1b_pi_mpi.cpp -o p1b
// Run:     mpirun -np 4 ./p1b
#include <iostream>
#include <cstdlib>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);

	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int n = 1000000;           // total points
	int local_n = n / size;    // points per process
	long local_count = 0, total_count = 0;

	double start = MPI_Wtime();

	srand(1234 + rank);        // different seed for every process
	for (int i = 0; i < local_n; i++) {
		double x = (double)rand() / RAND_MAX;
		double y = (double)rand() / RAND_MAX;
		if (x * x + y * y <= 1.0) local_count++;
	}

	// add partial counts of all processes into process 0
	MPI_Reduce(&local_count, &total_count, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		double time = MPI_Wtime() - start;
		cout << "Processes: " << size << endl;
		cout << "Points   : " << local_n * size << endl;
		cout << "PI       : " << 4.0 * total_count / (local_n * size) << endl;
		cout << "Time     : " << time << " s" << endl;
	}

	MPI_Finalize();
}
