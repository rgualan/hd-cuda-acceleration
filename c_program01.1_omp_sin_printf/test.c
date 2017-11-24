#include <stdio.h>
#include <omp.h>

/**
 * main program
 */
void main() {

	int n = omp_get_num_threads();

	printf("omp_get_num_threads: %d \n", n);
}

