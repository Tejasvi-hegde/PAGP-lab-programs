// Program 3: Cache-unfriendly vs cache-friendly (segmented) Sieve of Eratosthenes
// Correctness: all versions must print the same prime count
//              (primes up to 100,000,000 = 5761455).
// Compile: g++ -O2 -fopenmp program3_sieve_cache.cpp -o p3
// Run:     ./p3
#include <iostream>
#include <cmath>
#include <algorithm>
#include <omp.h>
using namespace std;

// marks multiples of 'step' as composite from 'start' to 'stop'
// returns the next multiple (where to continue later)
long strike(bool comp[], long start, long step, long stop) {
	long i;
	for (i = start; i <= stop; i += step)
		comp[i] = true;
	return i;
}

// 1. Cache unfriendly: strikes each prime over the WHOLE array
long unfriendly(long n) {
	long m = sqrt(n), count = 0;
	bool *comp = new bool[n + 1]();   // () sets all to false
	double t = omp_get_wtime();

	for (long i = 2; i <= m; i++)
		if (!comp[i]) strike(comp, 2 * i, i, n);
	for (long i = 2; i <= n; i++)
		if (!comp[i]) count++;

	cout << "Unfriendly time: " << omp_get_wtime() - t << " s" << endl;
	delete[] comp;
	return count;
}

// 2. Cache friendly: works on one block of size sqrt(n) at a time
long friendly(long n) {
	long m = sqrt(n), count = 0, nfac = 0;
	bool *comp = new bool[n + 1]();
	long *factor = new long[m + 1];
	long *striker = new long[m + 1];
	double t = omp_get_wtime();

	for (long i = 2; i <= m; i++)
		if (!comp[i]) {
			count++;
			factor[nfac] = i;
			striker[nfac] = strike(comp, 2 * i, i, m);
			nfac++;
		}
	for (long left = m + 1; left <= n; left += m) {
		long right = min(left + m - 1, n);
		for (long k = 0; k < nfac; k++)
			striker[k] = strike(comp, striker[k], factor[k], right);
		for (long i = left; i <= right; i++)
			if (!comp[i]) count++;
	}

	cout << "Friendly   time: " << omp_get_wtime() - t << " s" << endl;
	delete[] comp; delete[] factor; delete[] striker;
	return count;
}

// 3. Cache friendly + parallel (OpenMP) inside every block
long parallel(long n) {
	long m = sqrt(n), count = 0, nfac = 0;
	bool *comp = new bool[n + 1]();
	long *factor = new long[m + 1];
	long *striker = new long[m + 1];
	double t = omp_get_wtime();

	for (long i = 2; i <= m; i++)
		if (!comp[i]) {
			count++;
			factor[nfac] = i;
			striker[nfac] = strike(comp, 2 * i, i, m);
			nfac++;
		}
	for (long left = m + 1; left <= n; left += m) {
		long right = min(left + m - 1, n);
		#pragma omp parallel for
		for (long k = 0; k < nfac; k++)
			striker[k] = strike(comp, striker[k], factor[k], right);
		#pragma omp parallel for reduction(+:count)
		for (long i = left; i <= right; i++)
			if (!comp[i]) count++;
	}

	cout << "Parallel   time: " << omp_get_wtime() - t << " s" << endl;
	delete[] comp; delete[] factor; delete[] striker;
	return count;
}

int main() {
	long n = 100000000;
	cout << "Primes (unfriendly): " << unfriendly(n) << endl;
	cout << "Primes (friendly)  : " << friendly(n) << endl;
	cout << "Primes (parallel)  : " << parallel(n) << endl;
}
