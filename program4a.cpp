// Program 4a: Convert a colour PNG image to black & white using OpenMP
// Try different schedules / chunk sizes WITHOUT editing the code, using OMP_SCHEDULE:
//   export OMP_SCHEDULE="static,10"     (or "dynamic,100", "guided,50" ...)
// Compile: g++ -fopenmp program4a_bw_image.cpp -o p4a -lgd
// Run:     ./p4a input.png output.png
#include <iostream>
#include <cstdio>
#include <omp.h>
#include <gd.h>
using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <in.png> <out.png>" << endl;
		return 1;
	}
	FILE *in = fopen(argv[1], "rb");
	if (!in) { cout << "Cannot open input file" << endl; return 1; }
	gdImagePtr img = gdImageCreateFromPng(in);
	fclose(in);
	if (!img) { cout << "Not a valid PNG" << endl; return 1; }
	gdImagePaletteToTrueColor(img);   // so pixels can hold any colour

	int w = gdImageSX(img), h = gdImageSY(img);
	double start = omp_get_wtime();

	#pragma omp parallel for schedule(runtime)
	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			int c = gdImageGetPixel(img, x, y);
			int gray = (gdImageRed(img, c) + gdImageGreen(img, c) + gdImageBlue(img, c)) / 3;
			gdImageSetPixel(img, x, y, gdTrueColor(gray, gray, gray));
		}
	}

	double time = omp_get_wtime() - start;

	FILE *out = fopen(argv[2], "wb");
	gdImagePng(img, out);
	fclose(out);
	gdImageDestroy(img);
	cout << "Time: " << time << " s" << endl;
}
