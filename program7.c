// Program 7: Matrix multiplication using OpenACC with dynamic memory allocation
// Compile: nvc -acc -o p7 program7_openacc_matrix.c      (or pgcc -acc)
// Run:     ./p7
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 1000

int main() {
	int n = SIZE;
	// dynamic allocation (A[i][j] -> A[i*n+j])
	double *a = (double *)malloc(n * n * sizeof(double));
	double *b = (double *)malloc(n * n * sizeof(double));
	double *c = (double *)malloc(n * n * sizeof(double));

	for (int i = 0; i < n * n; i++) {
		a[i] = i % 10;
		b[i] = (i * 2) % 10;
	}

	struct timeval s, e;
	gettimeofday(&s, NULL);

	// copy a and b to the GPU, copy c back after computing
	#pragma acc parallel loop collapse(2) copyin(a[0:n*n], b[0:n*n]) copyout(c[0:n*n])
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			double sum = 0;
			for (int k = 0; k < n; k++)
				sum += a[i*n + k] * b[k*n + j];
			c[i*n + j] = sum;
		}

	gettimeofday(&e, NULL);
	printf("Time: %f s\n", (e.tv_sec - s.tv_sec) + (e.tv_usec - s.tv_usec) / 1e6);
	printf("c[0] = %f, c[last] = %f\n", c[0], c[n*n - 1]);

	free(a); free(b); free(c);
	return 0;
}
