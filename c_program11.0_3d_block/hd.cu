#include <stdio.h>
#include "hd_block_size.h"

#define TODEV(A,s) float *A##_d;cudaMalloc((void**)&A##_d,((s))*sizeof(float));cudaMemcpy(A##_d,A,(s)*sizeof(float),cudaMemcpyHostToDevice);
#define FROMDEV(A,s) cudaMemcpy(A,A##_d,(s)*sizeof(float),cudaMemcpyDeviceToHost);
#define CLNUP(A) cudaFree(A##_d)

#define TODEV3(A) TODEV(A,d3)
#define TODEV2(A) TODEV(A,d2)
#define FROMDEV3(A) FROMDEV(A,d3)
#define FROMDEV2(A) FROMDEV(A,d2)

//extern __constant__ float rdx_[1];
//extern __constant__ float rdy_[1];

__global__ void horizontal_diffusion_gpu(const int ids, const int ide,
		const int jds, const int jde, const int kds, const int kde,
		const int ims, const int ime, const int jms, const int jme,
		const int kms, const int kme, const int its, const int ite,
		const int jts, const int jte, const int kts, const int kte,
		const int cf_specified, const int cf_nested, const int cf_open_xs,
		const int cf_open_xe, const int cf_open_ys, const int cf_open_ye,
		const int cf_periodic_x, const int cf_polar, const char name,
		const float *field, float *tendency, const float *mu,
		const float *msfux, const float *msfuy, const float *msfvx,
		const float *msfvx_inv, const float *msfvy, const float *msftx,
		const float *msfty, const float khdif, const float *xkmhd,
		const float rdx, const float rdy);

extern "C" int gethostname(char * name, size_t len);

extern "C" {

/**
 * Gets some basic device information,
 * sets the device for the task,
 * and performs a simply alloc and transfer operation on GPU
 */
int horizontal_diffusion_gpu_init_(int *myproc, int *nproc, int *mydevice) {
	float x, *x_d;
	int i, dc;
	cudaError_t cerr;
	char hostname[64];
	cudaDeviceProp dp;

	cudaEvent_t tS, tE;
	float timer = 0.0f;
	cudaEventCreate(&tS);
	cudaEventCreate(&tE);

	// Get some GPU device info
	cudaGetDeviceCount(&dc);
	if (dc > 4) {
		fprintf(stderr, "Warning: more than %d devices on node (%d)\n", 4, dc);
		dc = 4;
	}

	fprintf(stderr, "Number of devices on this node: %d\n", dc);
	i = (*mydevice);
	if (dc > 0) {
		if ((cerr = cudaSetDevice(i))) {
			fprintf(stderr, "Non-zero cerr %d\n", cerr);
		}
	}

	gethostname(hostname, 64);
	fprintf(stderr, "Setting device %02d for task %03d on host %s\n", i,
			*myproc, hostname);
	if ((cerr = cudaGetDeviceProperties(&dp, i))) {
		fprintf(stderr, "Device %02d: cerr = %d\n", i, cerr);
	} else {
		fprintf(stderr, "Device %02d: name %s\n", i, dp.name);
		fprintf(stderr, "Device %02d: mem       %lu\n", i, dp.totalGlobalMem);
		fprintf(stderr, "Device %02d: smem      %lu\n", i,
				dp.sharedMemPerBlock);
		fprintf(stderr, "Device %02d: nreg      %d\n", i, dp.regsPerBlock);
		fprintf(stderr, "Device %02d: warp      %d\n", i, dp.warpSize);
		fprintf(stderr, "Device %02d: pitch     %lu\n", i, dp.memPitch);
		fprintf(stderr, "Device %02d: maxthrds  %d\n", i,
				dp.maxThreadsPerBlock);
		fprintf(stderr, "Device %02d: maxtdim   %d %d %d\n", i,
				(dp.maxThreadsDim)[0], (dp.maxThreadsDim)[1],
				(dp.maxThreadsDim)[2]);
		fprintf(stderr, "Device %02d: maxgdim   %d %d %d\n", i,
				(dp.maxGridSize)[0], (dp.maxGridSize)[1], (dp.maxGridSize)[2]);
		fprintf(stderr, "Device %02d: clock     %d\n", i, dp.clockRate);
		fprintf(stderr, "Device %02d: talign    %lu\n", i, dp.textureAlignment);
	}

	cudaEventRecord(tS, NULL);

	cudaMalloc((void **) (&x_d), sizeof(float));
	cudaMemcpy(x_d, &x, sizeof(float), cudaMemcpyHostToDevice);
	cudaFree(x_d);

	cudaEventRecord(tE, NULL);
	cudaEventSynchronize(tE);
	cudaEventElapsedTime(&timer, tS, tE);

	fprintf(stderr, "horizontal_diffusion_gpu_init: %.3f\n", timer);

	return 0;
}

/**
 * Convert fortran index to c index
 */
int indexI(int fi) {
	return fi + 4;
}
int indexJ(int fj) {
	return fj + 4;
}
int indexK(int fk) {
	return fk - 1;
}

// Dimensiones de las variables
int IMS = -4;
int IME = 430;
int JMS = -4;
int JME = 305;
int KMS = 1;
int KME = 35;
int IX = 435;
int JX = 310;
int KX = 35;

/**
 * Print variable to console
 */
void printVariable(const char name[], float *var, int ims, int ime, int kms,
		int kme, int jms, int jme) {
	printf("%s:\n", name);
	for (int k = indexK(kms); k <= indexK(kme); k++) {
		for (int j = indexJ(jms); j <= indexJ(jme); j++) {
			for (int i = indexI(ims); i <= indexI(ime); i++) {
				printf("%7.2f\t", var[i + k * IX + j * IX * KX]);
			}
			printf("\n");
		}
		printf("*\n");
	}
}

//[435][35][310]
int horizontal_diffusion_host_(int *ids, int *ide, int *jds, int *jde, int *kds,
		int *kde, int *ims, int *ime, int *jms, int *jme, int *kms, int *kme,
		int *its, int *ite, int *jts, int *jte, int *kts, int *kte,
		int *cf_specified, int *cf_nested, int *cf_open_xs, int *cf_open_xe,
		int *cf_open_ys, int *cf_open_ye, int *cf_periodic_x, int *cf_polar,
		char *name, float *field, float *tendency, float *mu, float *msfux,
		float *msfuy, float *msfvx, float *msfvx_inv, float *msfvy,
		float *msftx, float *msfty, float *khdif, float *xkmhd, float *rdx,
		float *rdy) {

	// Dimensions
	int d3 = (*ime - *ims + 1) * (*jme - *jms + 1) * (*kme - *kms + 1);
	int d2 = (*ime - *ims + 1) * (*jme - *jms + 1);

	// Timing data
	cudaEvent_t tS0, tE0, tS1, tE1;
	cudaEventCreate(&tS0);
	cudaEventCreate(&tS1);
	cudaEventCreate(&tE0);
	cudaEventCreate(&tE1);
	float timer = 0.0f;

	printf("Dimensions:\n");
	printf("ids,ide,jds,jde,kds,kde: %d,%d,%d,%d,%d,%d\n", *ids, *ide, *jds,
			*jde, *kds, *kde);
	printf("ims,ime,jms,jme,kms,kme: %d,%d,%d,%d,%d,%d\n", *ims, *ime, *jms,
			*jme, *kms, *kme);
	printf("its,ite,jts,jte,kts,kte: %d,%d,%d,%d,%d,%d\n", *its, *ite, *jts,
			*jte, *kts, *kte);

	/*printf("Input variables: \n");
	 printf("Boolean: %d %d %d %d %d %d %d %d\n", *cf_specified, *cf_nested, *cf_open_xs, *cf_open_xe, *cf_open_ys, *cf_open_ye, *cf_periodic_x, *cf_polar);
	 printf("String: %c\n", *name);
	 printf("Float: %f %f %f\n", *khdif, *rdx, *rdy);*/

	// Starting transference of data to device memory
	cudaEventRecord(tS0, NULL);

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

	// Main variable - before
	//printVariable("Tendency (input)", tendency, 100, 120, 1, 1, 200, 200);

	int remx, remy;
	remx = (*ime - *ims + 1) % XXX != 0 ? 1 : 0;
	remy = (*jme - *jms + 1) % YYY != 0 ? 1 : 0;

	dim3 dimBlock(XXX, YYY, 1);
	dim3 dimGrid(((*ime - *ims + 1) / XXX) + remx,
			((*jme - *jms + 1) / YYY) + remy, MKX);

	printf("Call to kernel: block dims %d %d %d\n", dimBlock.x, dimBlock.y,
			dimBlock.z);
	printf("Call to kernel: grid  dims %d %d %d\n", dimGrid.x, dimGrid.y,
			dimGrid.z);
	printf("Calling kernel \n");

	cudaEventRecord(tS1, NULL);

	// Changing cache configuration
	cudaFuncSetCacheConfig(horizontal_diffusion_gpu, cudaFuncCachePreferL1);

	// Set constant memory
	//cudaMemcpyToSymbol(&rdx_, &rdx, sizeof(float));
	//cudaMemcpyToSymbol(&rdy_, &rdy, sizeof(float));

//for( int i=0; i<10; i++){

	cudaError_t err = cudaSuccess;
	horizontal_diffusion_gpu<<<dimGrid, dimBlock>>>(*ids, *ide, *jds, *jde,
			*kds, *kde, *ims, *ime, *jms, *jme, *kms, *kme, *its, *ite, *jts,
			*jte, *kts, *kte, *cf_specified, *cf_nested, *cf_open_xs,
			*cf_open_xe, *cf_open_ys, *cf_open_ye, *cf_periodic_x, *cf_polar,
			*name, field_d, tendency_d, mu_d, msfux_d, msfuy_d, msfvx_d,
			msfvx_inv_d, msfvy_d, msftx_d, msfty_d, *khdif, xkmhd_d, *rdx,
			*rdy);
	err = cudaGetLastError();
	if (err != cudaSuccess) {
		fprintf(stderr, "Failed to launch kernel (error code %s)!\n",
				cudaGetErrorString(err));
		exit (EXIT_FAILURE);
	}

	cudaThreadSynchronize();

//}
	cudaEventRecord(tE1, NULL);
	cudaEventSynchronize(tE1);
	float timerRun;
	cudaEventElapsedTime(&timerRun, tS1, tE1);

	fprintf(stderr, "Call to kernel (not including data xfer): %.3f msec\n",
			timerRun);
	//fprintf(stderr, "Call to kernel (not including data xfer): %.3f msec\n", timerRun/10);

	// Starting transference of output data from device
	FROMDEV3(tendency);

	cudaEventRecord(tE0, NULL);
	cudaEventSynchronize(tE0);
	cudaEventElapsedTime(&timer, tS0, tE0);
	printf("Call to kernel (including data xfer): %.3f msec\n", timer);
	//printf("Call to kernel (including data xfer): %.3f msec\n", timer - timerRun + (timerRun/10));

	//printVariable("(hd.cu): Tendency (output)", tendency, 100, 120, 1, 1, 200, 200);

	CLNUP(field);
	CLNUP(tendency);
	CLNUP(xkmhd);
	CLNUP(mu);
	CLNUP(msfux);
	CLNUP(msfuy);
	CLNUP(msfvx);
	CLNUP(msfvx_inv);
	CLNUP(msfvy);
	CLNUP(msftx);
	CLNUP(msfty);

	return 0;
}

/*
 int get_horizontal_diffusion_gpu_levels(int *retval) {
 (*retval) = MKX;
 }
 */

}

