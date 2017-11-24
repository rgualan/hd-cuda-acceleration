#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "spt1.h"
#include "multiSerial.h"

// Moving data
#if 0
#define TODEV(A,s) float *A##_d;cudaMalloc((void**)&A##_d,((s))*sizeof(float));cudaMemcpy(A##_d,A,(s)*sizeof(float),cudaMemcpyHostToDevice);
#define FROMDEV(A,s) cudaMemcpy(A,A##_d,(s)*sizeof(float),cudaMemcpyDeviceToHost);
#define CLNUP(A) cudaFree(A##_d)

#define TODEV3(A) TODEV(A,d3)
#define TODEV2(A) TODEV(A,d2)
#define FROMDEV3(A) FROMDEV(A,d3)
#define FROMDEV2(A) FROMDEV(A,d2)
#endif

#define TODEV(A,s,d) float *A##_d=malloc((d)*sizeof(float));memcpy(A##_d, &A[(s)], (d)*sizeof(float))
#define TODEV3(A) TODEV(A,s3,d3_)
#define TODEV2(A) TODEV(A,s2,d2_)

// Max number of GPUs used
#define MAX_GPU_COUNT 32

int horizontal_diffusion_host(int *ids, int *ide, int *jds, int *jde, int *kds,
		int *kde, int *ims, int *ime, int *jms, int *jme, int *kms, int *kme,
		int *its, int *ite, int *jts, int *jte, int *kts, int *kte,
		int *cf_specified, int *cf_nested, int *cf_open_xs, int *cf_open_xe,
		int *cf_open_ys, int *cf_open_ye, int *cf_periodic_x, int *cf_polar,
		char *name, float *field, float *tendency, float *mu, float *msfux,
		float *msfuy, float *msfvx, float *msfvx_inv, float *msfvy,
		float *msftx, float *msfty, float *khdif, float *xkmhd, float *rdx,
		float *rdy) {

	int GPU_N; // Number of GPUs

	// Dimensions (used in directives)
	// Original dimensions
	int d3 = (*ime - *ims + 1) * (*jme - *jms + 1) * (*kme - *kms + 1);
	int d2 = (*ime - *ims + 1) * (*jme - *jms + 1);

	printf("Dimensions:\n");
	printf("ids,ide,jds,jde,kds,kde: %4d,%4d,%4d,%4d,%4d,%4d\n", *ids, *ide,
			*jds, *jde, *kds, *kde);
	printf("ims,ime,jms,jme,kms,kme: %4d,%4d,%4d,%4d,%4d,%4d\n", *ims, *ime,
			*jms, *jme, *kms, *kme);
	printf("its,ite,jts,jte,kts,kte: %4d,%4d,%4d,%4d,%4d,%4d\n", *its, *ite,
			*jts, *jte, *kts, *kte);

	// Solver configuration
	// This structure has the basic I/O data for each calculation of a kernel
	TGPUplan plan[MAX_GPU_COUNT];

	// Get the number of available devices in the node
	// The limit is MAX_GPU_COUNT
	/*checkCudaErrors(cudaGetDeviceCount(&GPU_N));
	 if (GPU_N > MAX_GPU_COUNT) {
	 GPU_N = MAX_GPU_COUNT;
	 }*/
	GPU_N = 2;
	printf("CUDA-capable device count: %i\n", GPU_N);

	// Subdivide workload magnitude
	int i;

	printf("Subdividing workload...\n");
	for (i = 0; i < GPU_N; i++) {
		plan[i].sub_dim_j = (*jme - *jms + 1) / GPU_N;
	}

	//Take into account "odd" data sizes
	for (i = 0; i < (*jme - *jms + 1) % GPU_N; i++) {
		plan[i].sub_dim_j++;
	}

	// Define indices for processing and for sub-data loading
	for (i = 0; i < GPU_N; i++) {
		// Sub-region processing indices
		if (i == 0) {
			plan[i].sub_js = *jms;
			plan[i].sub_je = plan[i].sub_js + plan[i].sub_dim_j - 1;
		} else {
			plan[i].sub_js = plan[i - 1].sub_je + 1;
			plan[i].sub_je = plan[i].sub_js + plan[i].sub_dim_j - 1;
		}
		printf("Subdivision of proc (GPU %d) in j dim: (%4d,%4d) (%d)\n", i,
				plan[i].sub_js, plan[i].sub_je, plan[i].sub_dim_j);

		// Sub-region data indices
		if (plan[i].sub_js - 1 < (*ims))
			plan[i].data_js = (*ims);
		else
			plan[i].data_js = plan[i].sub_js - 1;

		if (plan[i].sub_je + 1 > (*jme))
			plan[i].data_je = (*jme);
		else
			plan[i].data_je = plan[i].sub_je + 1;
		printf("Subdivision of data (GPU %d) in j dim: (%4d,%4d) (%d)\n", i,
				plan[i].data_js, plan[i].data_je,
				(plan[i].data_je - plan[i].data_js + 1));
	}

	//Create streams for issuing GPU command asynchronously and allocate memory (GPU and System page-locked)
	for (i = 0; i < GPU_N; i++) {
		//checkCudaErrors(cudaSetDevice(i));
		//checkCudaErrors(cudaStreamCreate(&plan[i].stream));
		//Allocate memory
		/*checkCudaErrors(
		 cudaMalloc((void **) &plan[i].d_Data,
		 plan[i].dataN * sizeof(float)));
		 checkCudaErrors(
		 cudaMalloc((void **) &plan[i].d_Sum, ACCUM_N * sizeof(float)));
		 checkCudaErrors(
		 cudaMallocHost((void **) &plan[i].h_Sum_from_device,
		 ACCUM_N * sizeof(float)));
		 checkCudaErrors(
		 cudaMallocHost((void **) &plan[i].h_Data,
		 plan[i].dataN * sizeof(float)));*/

		// Initialize
		/*for (j = 0; j < plan[i].dataN; j++) {
		 plan[i].h_Data[j] = (float) rand() / (float) RAND_MAX;
		 }*/

		//Start timing and compute on GPU(s)
	}

	printf("Computing with %d GPUs...\n", GPU_N);
	//StartTimer();

	//Copy data to GPU, launch the kernel and copy data back. All asynchronously
	for (i = 0; i < GPU_N; i++) {
		//Set device
		//checkCudaErrors(cudaSetDevice(i));

		//Copy input data from CPU
		/*checkCudaErrors(
		 cudaMemcpyAsync(plan[i].d_Data, plan[i].h_Data,
		 plan[i].dataN * sizeof(float), cudaMemcpyHostToDevice,
		 plan[i].stream));*/

		startTimer(1);
		// Starting indices for coping data from original variable
		int s3 = J(plan[i].data_js) * ((*ime - *ims + 1) * (*kme - *kms + 1));
		int s2 = J(plan[i].data_js) * ((*ime - *ims + 1));

		// Dimension of new partial variables
		int d3_ = (*ime - *ims + 1) * (*kme - *kms + 1)
				* (plan[i].data_je - plan[i].data_js + 1);
		int d2_ = (*ime - *ims + 1) * (plan[i].data_je - plan[i].data_js + 1);

		// Create a partial copy of the variables for sending to device

		TODEV3(field);
		TODEV3(tendency);
		TODEV3(xkmhd);
		TODEV2(mu);
		TODEV2(msfux);
		TODEV2(msfuy);
		TODEV2(msfvx);
		TODEV2(msfvx_inv);
		TODEV2(msfvy);
		TODEV2(msftx);
		TODEV2(msfty);

		//Perform GPU computations
		/*reduceKernel<<<BLOCK_N, THREAD_N, 0, plan[i].stream>>>(plan[i].d_Sum,
		 *
		 plan[i].d_Data, plan[i].dataN);*/
		//getLastCudaError("reduceKernel() execution failed.\n");
		//Read back GPU results
		/*checkCudaErrors(
		 cudaMemcpyAsync(plan[i].h_Sum_from_device, plan[i].d_Sum,
		 ACCUM_N * sizeof(float), cudaMemcpyDeviceToHost,
		 plan[i].stream));*/

		printf("Computing with GPU No %d...\n", i);
		startTimer(2);

		horizontal_diffusion_kernel(ids, ide, jds, jde, kds, kde, ims, ime, jms,
				jme, kms, kme, its, ite, jts, jte, kts, kte, cf_specified,
				cf_nested, cf_open_xs, cf_open_xe, cf_open_ys, cf_open_ye,
				cf_periodic_x, cf_polar, name, field_d, tendency_d, mu_d,
				msfux_d, msfuy_d, msfvx_d, msfvx_inv_d, msfvy_d, msftx_d,
				msfty_d, khdif, xkmhd_d, rdx, rdy, plan[i].sub_js,
				plan[i].sub_je, plan[i].data_js, plan[i].data_je);

		stopTimer(2);

		s3 = J(plan[i].sub_js) * ((*ime - *ims + 1) * (*kme - *kms + 1));
		s2 = J(plan[i].sub_js) * ((*ime - *ims + 1));
		d3_ = (*ime - *ims + 1) * (*kme - *kms + 1)
				* (plan[i].sub_je - plan[i].sub_js + 1);
		int delta = (plan[i].sub_js - plan[i].data_js) * (*ime - *ims + 1)
				* (*kme - *kms + 1);

		memcpy(&tendency[s3], &tendency_d[delta], (d3_) * sizeof(float));

		stopTimer(1);
		printf(
				"Time for running horizontal_diffusion_kernel (w/o IO) : %d ms.\n",
				getElapsedTimeMs(2));
		printf(
				"Time for running horizontal_diffusion_kernel (w IO)   : %d ms.\n",
				getElapsedTimeMs(1));
	}

	return 0;
}

