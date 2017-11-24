#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "spt2.h"

/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract(struct timeval *result, struct timeval *t2,
		struct timeval *t1) {
	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec)
			- (t1->tv_usec + 1000000 * t1->tv_sec);
	result->tv_sec = diff / 1000000;
	result->tv_usec = diff % 1000000;

	return (diff < 0);
}

void timeval_print(struct timeval *tv) {
	char buffer[30];
	time_t curtime;

	printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
	curtime = tv->tv_sec;
	strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
	printf(" = %s.%06ld\n", buffer, tv->tv_usec);
}

/**
 * Reads <dim> elements of the <var>
 * from <stream>
 */
void readArray(FILE *stream, float *var, const int dim) {
	int i;
	int res;
	for (i = 0; i < dim; i++) {
		res = fscanf(stream, "%f ", &var[i]);
		if (res == 0) {
			printf("Error while reading array...\n");
			exit(1);
		}
	}
}

/**
 * Creates a text file with a snapshot of the matrix <var>
 */
void create_snapshot(char * file_name, float *var, const int ix, const int jx,
		const int kx) {
	FILE *f = fopen(file_name, "w");
	//fwrite(var, sizeof(float), dim, f);

	int i, j;
	for (j = 0; j < jx; j++) {
		for (i = 0; i < ix; i++) {
			fprintf(f, "%f\t", var[P3(i,0,j)]);
		}
		fprintf(f, "\n");
	}

	fclose(f);
}

/**
 * main program
 */
void main() {
	// For timing events
	int msec;
	clock_t start, end;
	// For file
	FILE *tp;

	// Model (1)
	int i, j, k;
	int ids, ide, jds, jde, kds, kde;
	int ims, ime, jms, jme, kms, kme;
	int its, ite, jts, jte, kts, kte;
	char cf_specified_, cf_nested_, cf_open_xs_, cf_open_xe_, cf_open_ys_,
			cf_open_ye_, cf_periodic_x_, cf_polar_;
	int cf_specified = 0, cf_nested = 0, cf_open_xs = 0, cf_open_xe = 0,
			cf_open_ys = 0, cf_open_ye = 0, cf_periodic_x = 0, cf_polar = 0;
	char name;

	/////////////////////////////////////////////
	// Reading
	printf("Leyendo parametros desde el archivo...\n");

	start = clock();
	tp = fopen("inputdata-00001X", "rt");

	fscanf(tp, "%d %d %d %d %d %d ", &ids, &ide, &jds, &jde, &kds, &kde);
	fscanf(tp, "%d %d %d %d %d %d ", &ims, &ime, &jms, &jme, &kms, &kme);
	fscanf(tp, "%d %d %d %d %d %d ", &its, &ite, &jts, &jte, &kts, &kte);
	fscanf(tp, "%c %c %c %c %c %c %c %c ", &cf_specified_, &cf_nested_,
			&cf_open_xs_, &cf_open_xe_, &cf_open_ys_, &cf_open_ye_,
			&cf_periodic_x_, &cf_polar_);
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

	// Model (2)
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

	fclose(tp);

	// copy tendency -> tendency_input (for comparisson purposes)
	memcpy(tendency_input, tendency, sizeof(tendency));

	// Print some read variables
	printf("Indices ids, ide, jds, jde, kds, kde : %d %d %d %d %d %d \n", ids,
			ide, jds, jde, kds, kde);
	printf("Indices ims, ime, jms, jme, kms, kme: %d %d %d %d %d %d \n", ims,
			ime, jms, jme, kms, kme);
	printf("Indices its, ite, jts, jte, kts, kte: %d %d %d %d %d %d \n", its,
			ite, jts, jte, kts, kte);
	printf("Dimensiones: %d %d %d \n", ix, jx, kx);
	printf("Banderas: %c %c %c %c %c %c %c %c \n", cf_specified_, cf_nested_,
			cf_open_xs_, cf_open_xe_, cf_open_ys_, cf_open_ye_, cf_periodic_x_,
			cf_polar_);
	printf("Name: %c \n", name);
	printf("rdx rdy: %f %f \n", rdx, rdy);

	end = clock();
	msec = (end - start) * 1000 / CLOCKS_PER_SEC;
	printf("Time for reading input variables: %d ms \n", msec);

	/////////////////////////////////////////////
	// Calling
	// Debug
	/*
	 for (i = I(1); i < I(100); i++) {
	 printf("%10.3f\t", field[P3(i,J(10),K(10))]);
	 //printf("%d\t", P3(i,10,10));
	 }
	 printf("\n");*/

	struct timeval tvBegin, tvEnd, tvDiff;

	printf("Calling horizontal_diffusion method \n");
	start = clock();
	gettimeofday(&tvBegin, NULL);
	timeval_print(&tvBegin);

	horizontal_diffusion_c_(&ids, &ide, &jds, &jde, &kds, &kde, &ims, &ime,
			&jms, &jme, &kms, &kme, &its, &ite, &jts, &jte, &kts, &kte,
			&cf_specified, &cf_nested, &cf_open_xs, &cf_open_xe, &cf_open_ys,
			&cf_open_ye, &cf_periodic_x, &cf_polar, &name, &field, &tendency,
			&mu, &msfux, &msfuy, &msfvx, &msfvx_inv, &msfvy, &msftx, &msfty,
			&khdif, &xkmhd, &rdx, &rdy);

	gettimeofday(&tvEnd, NULL);
	timeval_print(&tvEnd);
	// diff
	timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
	printf("%ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
	printf("Time for running horizontal_diffusion (gettimeofday): %ld ms \n",
			((tvDiff.tv_sec) * 1000 + (tvDiff.tv_usec) / 1000));

	end = clock();
	msec = (end - start) * 1000 / CLOCKS_PER_SEC;
	printf("Time for running horizontal_diffusion: %d ms \n", msec);

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
				if (abs(tendency_target[P3(i, k, j)] - tendency[P3(i, k, j)])
						> 1.0) {
					countDiff = countDiff + 1;
				}

				// Calc max error from target
				if (abs(tendency_target[P3(i, k, j)] - tendency[P3(i, k, j)])
						> maxError) {
					maxError = abs(
							tendency_target[P3(i, k, j)]
									- tendency[P3(i, k, j)]);
				}

			}
		}
	}

	printf(
			"Big differences between target-tendency and CALC-tendency: %d / %d \n",
			countDiff, dim3d);
	printf("Max point-to-point error: %f \n", maxError);

	//Export output snapshot for comparison
	printf("Creating snapshots...\n");
	create_snapshot("outputdataCalc.txt", tendency, ix, jx, kx);
	create_snapshot("outputdataTarget.txt", tendency_target, ix, jx, kx);
	create_snapshot("outputdataInput.txt", tendency_input, ix, jx, kx);

	return;
}

