#ifndef RGUALAN_UTIL
#define RGUALAN_UTIL

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "spt1.h"

// Return 1 if the difference is negative, otherwise 0.
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

struct timeval t[10]; // Maximum 5 simultaneous timers
int offset = 5;

void validateIndex(int i) {
	if (!(i >= 0 && i <= 4)) {
		printf("Error. Maximum 5 simulateous timers are allowed (0-4).\n");
		exit(1);
	}

}

void startTimer(int i) {
	validateIndex(i);
	gettimeofday(&t[i], NULL );
}

void stopTimer(int i) {
	validateIndex(i);
	gettimeofday(&t[i + offset], NULL );
}

// Return the elapsed time in ms

int getElapsedTimeMs(int i) {
	validateIndex(i);

	// compute and print the elapsed time in millisec
	int elapsedTime;
	struct timeval t1 = t[i];
	struct timeval t2 = t[i+offset];

	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

	return elapsedTime;
}

#endif
