#include <stdio.h>
#include <string.h>
#include "spt1.h"
#include "util.h"

////////////////////////////////////////////////////////////////////////////////////////
// This programs aims to divide the horizontal_diffusion method
// using CPU computing
// as a previous step
// to convert the method to CUDA for using several GPUs

/////////////////////////////////////////////
// Main program
int main() {
	// File for reading input file
	FILE *tp;
	char *fileName = "inputdata-00001X";

	// Model (part 1)
	int i, j, k;
	int ids, ide, jds, jde, kds, kde;
	int ims, ime, jms, jme, kms, kme;
	int its, ite, jts, jte, kts, kte;
	char cf_specified_, cf_nested_, cf_open_xs_, cf_open_xe_, cf_open_ys_, cf_open_ye_,
			cf_periodic_x_, cf_polar_;
	int cf_specified = 0, cf_nested = 0, cf_open_xs = 0, cf_open_xe = 0, cf_open_ys = 0,
			cf_open_ye = 0, cf_periodic_x = 0, cf_polar = 0;
	char name;

	/////////////////////////////////////////////
	// Reading
	printf("Reading parameters from input file %s...\n", fileName);

	startTimer(0);
	tp = fopen(fileName, "rt");

	fscanf(tp, "%d %d %d %d %d %d ", &ids, &ide, &jds, &jde, &kds, &kde);
	fscanf(tp, "%d %d %d %d %d %d ", &ims, &ime, &jms, &jme, &kms, &kme);
	fscanf(tp, "%d %d %d %d %d %d ", &its, &ite, &jts, &jte, &kts, &kte);
	fscanf(tp, "%c %c %c %c %c %c %c %c ", &cf_specified_, &cf_nested_, &cf_open_xs_, &cf_open_xe_,
			&cf_open_ys_, &cf_open_ye_, &cf_periodic_x_, &cf_polar_);
	fscanf(tp, "%c ", &name);

	if (cf_specified_ == 'T')
		cf_specified = 1;
	if (cf_nested_ == 'T')
		cf_nested = 1;
	if (cf_open_xs_ == 'T')
		cf_open_xs = 1;
	if (cf_open_xe_ == 'T')
		cf_open_xe = 1;
	if (cf_open_ys == 'T')
		cf_open_ys = 1;
	if (cf_open_ye_ == 'T')
		cf_open_ye = 1;
	if (cf_periodic_x_ == 'T')
		cf_periodic_x = 1;
	if (cf_polar_ == 'T')
		cf_polar = 1;

	// Model (part 2)
	const int ix = ime - ims + 1;
	const int jx = jme - jms + 1;
	const int kx = kme - kms + 1;
	const int dim3d = ix * jx * kx;
	const int dim2d = ix * jx;
	printf("Number of elements of 3d arrays: %d \n", dim3d);
	printf("Number of elements of 2d arrays: %d \n", dim2d);

	float field[dim3d];
	float tendency[dim3d];
	float xkmhd[dim3d];
	float tendency_target[dim3d];
	float tendency_input[dim3d];

	float mu[dim2d];
	float msfux[dim2d];
	float msfuy[dim2d];
	float msfvx[dim2d];
	float msfvx_inv[dim2d];
	float msfvy[dim2d];
	float msftx[dim2d];
	float msfty[dim2d];

	float rdx, rdy, khdif;

	readArray(tp, field, dim3d);
	readArray(tp, tendency, dim3d);
	readArray(tp, mu, dim2d);
	readArray(tp, msfux, dim2d);
	readArray(tp, msfuy, dim2d);
	readArray(tp, msfvx, dim2d);
	readArray(tp, msfvx_inv, dim2d);
	readArray(tp, msfvy, dim2d);
	readArray(tp, msftx, dim2d);
	readArray(tp, msfty, dim2d);

	readArray(tp, &khdif, 1);
	readArray(tp, xkmhd, dim3d);
	readArray(tp, &rdx, 1);
	readArray(tp, &rdy, 1);

	fclose(tp); // Close file

	// copy tendency -> tendency_input (for comparison purposes)
	memcpy(tendency_input, tendency, sizeof(tendency));

	// Print some read variables
	printf("Indices ids, ide, jds, jde, kds, kde: %4d %4d %4d %4d %4d %4d \n", ids, ide, jds, jde,
			kds, kde);
	printf("Indices ims, ime, jms, jme, kms, kme: %4d %4d %4d %4d %4d %4d \n", ims, ime, jms, jme,
			kms, kme);
	printf("Indices its, ite, jts, jte, kts, kte: %4d %4d %4d %4d %4d %4d \n", its, ite, jts, jte,
			kts, kte);
	printf("Dimensions: %d %d %d \n", ix, jx, kx);
	printf("Flags: %c %c %c %c %c %c %c %c \n", cf_specified_, cf_nested_, cf_open_xs_, cf_open_xe_,
			cf_open_ys_, cf_open_ye_, cf_periodic_x_, cf_polar_);
	printf("Name: %c \n", name);
	printf("Float: rdx rdy: %f %f \n", rdx, rdy);

	stopTimer(0);
	printf("Time for reading input variables: %d ms.\n", getElapsedTimeMs(0));

	/////////////////////////////////////////////
	// Calling horizontal_diffusion host
	printf("Calling horizontal_diffusion method \n");
	startTimer(0);

	horizontal_diffusion_host(&ids, &ide, &jds, &jde, &kds, &kde, &ims, &ime, &jms, &jme, &kms,
			&kme, &its, &ite, &jts, &jte, &kts, &kte, &cf_specified, &cf_nested, &cf_open_xs,
			&cf_open_xe, &cf_open_ys, &cf_open_ye, &cf_periodic_x, &cf_polar, &name, &field,
			&tendency, &mu, &msfux, &msfuy, &msfvx, &msfvx_inv, &msfvy, &msftx, &msfty, &khdif,
			&xkmhd, &rdx, &rdy);

	stopTimer(0);
	printf("Time for running horizontal_diffusion host: %d ms.\n", getElapsedTimeMs(0));

	/////////////////////////////////////////////
	// Assessing output
	printf("Assessing output...\n");
	char *out_fname = "outputdata-00001";
	printf("Reading output variable from file: %s...\n", out_fname);
	tp = fopen(out_fname, "rt");
	readArray(tp, tendency_target, dim3d);
	fclose(tp);

	int countDiff = 0;
	float maxError = 0;

	for (i = 0; i < ix; i++) {
		for (j = 0; j < jx; j++) {
			for (k = 0; k < kx; k++) {
				// Count BIG differences
				if (abs(tendency_target[P3(i, k, j)] - tendency[P3(i, k, j)]) > 1.0) {
					countDiff = countDiff + 1;
				}

				// Calculate max error from target
				if (abs(tendency_target[P3(i, k, j)] - tendency[P3(i, k, j)]) > maxError) {
					maxError = abs(tendency_target[P3(i, k, j)] - tendency[P3(i, k, j)]);
				}

			}
		}
	}

	printf("Big differences between target-tendency and CALC-tendency: %d / %d \n", countDiff,
			dim3d);
	printf("Max point-to-point error: %f \n", maxError);

	//Export output snapshot for comparison
	printf("Creating snapshots...\n");
	create_snapshot("outputdataCalc.txt", tendency, ix, jx, kx);
	create_snapshot("outputdataTarget.txt", tendency_target, ix, jx, kx);
	create_snapshot("outputdataInput.txt", tendency_input, ix, jx, kx);

	printf("Done!\n");
	return 0;
}

