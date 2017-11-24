#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NR 5000                 // number of rows
#define NC 1000                 // number of columns

int main(int argc, char *argv[]) {
	int tid, nthreads, i, j, k, chunk;
	double a[NR][NC], /* matrix A to be multiplied */
	b[NR][NC], /* matrix B to be multiplied */
	c[NR][NC]; /* result matrix C */

	// chunk = 100; /* set loop iteration chunk size */

	/*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
	{
		chunk = NR/omp_get_num_threads();

		tid = omp_get_thread_num();
		if (tid == 0) {
			nthreads = omp_get_num_threads();
			printf("Starting matrix multiple example with %d threads\n",
					nthreads);
			printf("Initializing matrices...\n");
		}
		/*** Initialize matrices ***/
#pragma omp for schedule (static, chunk)
		for (i = 0; i < NR; i++)
			for (j = 0; j < NC; j++){
				a[i][j] = i + j;
				b[i][j] = i * j;
				c[i][j] = 0;
			}

		/*** Do matrix multiply sharing iterations on outer loop ***/
		/*** Display who does which iterations for demonstration purposes ***/
		printf("Thread %d starting matrix multiply...\n", tid);
#pragma omp for schedule (static, chunk)
		for (i = 0; i < NR; i++) {
			//printf("Thread=%d did row=%d\n", tid, i);
			for (j = 0; j < NC; j++)
				c[i][j] = a[i][j] * b[i][j];
		}
	} /*** End of parallel region ***/

	/*** Print results ***/
	/*
	printf("******************************************************\n");
	printf("Result Matrix:\n");
	for (i = 0; i < NR; i++) {
		for (j = 0; j < NC; j++)
			printf("%6.2f   ", c[i][j]);
		printf("\n");
	}
	printf("******************************************************\n");
	printf("Done.\n");
	*/
}
