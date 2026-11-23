// Program 2: Matrix multiplication using OpenMP and dynamic memory allocation
// (a) correctness: parallel result is compared with serial result
// (b) outer loop parallelised WITH and WITHOUT explicit data scope (private)
// Compile: g++ -fopenmp program2_matrix_multiplication.cpp -o p2
// Run:     ./p2
#include <iostream>
#include <omp.h>
using namespace std;

int main() {
	int n, threads;
	cout << "Enter matrix size: ";
	cin >> n;
	cout << "Enter number of threads: ";
	cin >> threads;
	omp_set_num_threads(threads);

	// dynamic allocation (matrix stored as 1D array: A[i][j] -> A[i*n+j])
	int *A = new int[n * n];
	int *B = new int[n * n];
	int *C = new int[n * n];
	int *S = new int[n * n];   // serial result, used to check correctness

	for (int i = 0; i < n * n; i++) {
		A[i] = i % 10;
		B[i] = (i * 2) % 10;
	}

	int i, j, k;   // declared outside the loops -> shared by default

	// ---- serial (reference) ----
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			S[i*n + j] = 0;
			for (k = 0; k < n; k++)
				S[i*n + j] += A[i*n + k] * B[k*n + j];
		}

	// ---- (b1) parallel WITHOUT private: j and k are shared -> data race ----
	double t = omp_get_wtime();
	#pragma omp parallel for
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			C[i*n + j] = 0;
			for (k = 0; k < n; k++)
				C[i*n + j] += A[i*n + k] * B[k*n + j];
		}
	t = omp_get_wtime() - t;
	bool ok1 = true;
	for (int x = 0; x < n * n; x++) if (C[x] != S[x]) { ok1 = false; break; }
	cout << "Without private(j,k): time " << t << " s, correct = " << (ok1 ? "YES" : "NO") << endl;

	// ---- (b2) parallel WITH private(j,k): every thread has its own j and k ----
	t = omp_get_wtime();
	#pragma omp parallel for private(j, k)
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			C[i*n + j] = 0;
			for (k = 0; k < n; k++)
				C[i*n + j] += A[i*n + k] * B[k*n + j];
		}
	t = omp_get_wtime() - t;
	bool ok2 = true;
	for (int x = 0; x < n * n; x++) if (C[x] != S[x]) { ok2 = false; break; }
	cout << "With    private(j,k): time " << t << " s, correct = " << (ok2 ? "YES" : "NO") << endl;

	delete[] A; delete[] B; delete[] C; delete[] S;
}
