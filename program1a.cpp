// Program 1a: Estimate PI using Monte Carlo (OpenMP)
// Compile: g++ -fopenmp program1a_pi_openmp.cpp -o p1a
// Run:     ./p1a
#include <iostream>
#include <cstdlib>
#include <omp.h>
using namespace std;

int main() {
	int threads, n;
	cout << "Enter number of threads: ";
	cin >> threads;
	cout << "Enter number of points: ";
	cin >> n;
	omp_set_num_threads(threads);

	long count = 0;  // points that fall inside the circle
	double start = omp_get_wtime();

	// each thread counts its own points; reduction adds the partial counts
	#pragma omp parallel reduction(+:count)
	{
		unsigned int seed = 1234 + omp_get_thread_num();  // private seed per thread
		#pragma omp for
		for (int i = 0; i < n; i++) {
			double x = (double)rand_r(&seed) / RAND_MAX;
			double y = (double)rand_r(&seed) / RAND_MAX;
			if (x * x + y * y <= 1.0) count++;
		}
	}

	double time = omp_get_wtime() - start;
	cout << "Threads : " << threads << endl;
	cout << "Points  : " << n << endl;
	cout << "PI      : " << 4.0 * count / n << endl;
	cout << "Time    : " << time << " s" << endl;
}
