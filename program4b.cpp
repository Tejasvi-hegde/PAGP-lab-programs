// Program 4b: Analyse scheduling by giving every thread its own colour.
// Thread 0 = blue, 1 = green, 2 = red, 3 = black (see the output image to
// understand which thread processed which columns).
// Change schedule using OMP_SCHEDULE, e.g.:
//   export OMP_SCHEDULE="static,50"   (or "dynamic,50", "guided,50")
// Compile: g++ -fopenmp program4b_thread_color.cpp -o p4b -lgd
// Run:     ./p4b output.png
#include <iostream>
#include <cstdio>
#include <omp.h>
#include <gd.h>
using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <out.png>" << endl;
		return 1;
	}
	int w = 1000, h = 1000;
	gdImagePtr img = gdImageCreateTrueColor(w, h);
	int colors[4] = { gdTrueColor(0, 0, 255), gdTrueColor(0, 255, 0),
	                  gdTrueColor(255, 0, 0), gdTrueColor(0, 0, 0) };

	double start = omp_get_wtime();

	#pragma omp parallel for num_threads(4) schedule(runtime)
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			gdImageSetPixel(img, x, y, colors[omp_get_thread_num() % 4]);

	double time = omp_get_wtime() - start;

	FILE *out = fopen(argv[1], "wb");
	gdImagePng(img, out);
	fclose(out);
	gdImageDestroy(img);
	cout << "Time: " << time << " s" << endl;
}
