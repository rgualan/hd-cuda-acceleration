#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cuda_runtime.h> // CUDA runtime
#include <helper_cuda.h>  // Utilities to work with CUDA
#include "spt1.h"
#include "multiGPU.h"
//#include "util.h"
#include "hd_block_size.h"

// Moving data
// To host (page-lock/pinned)
//#define TOHOST(A,d,s) checkCudaErrors(cudaMallocHost((void**)&plan[i].A##_h,(d)*sizeof(float)));memcpy(&plan[i].A##_h[0], &A[(s)], (d)*sizeof(float))
#define TOHOST(A,d,s) cudaMallocHost((void**)&plan[i].A##_h,(d)*sizeof(float));memcpy(&plan[i].A##_h[0], &A[(s)], (d)*sizeof(float))
#define TOHOST3(A) TOHOST(A,plan[i].d3,plan[i].s3)
#define TOHOST2(A) TOHOST(A,plan[i].d2,plan[i].s2)

// To device, from pinned memory
//#define TODEV(A,d) printf("TODEV:%d\n",d);checkCudaErrors(cudaMalloc((void**)&plan[i].A##_d,(d)*sizeof(float)));printf("Pasa cudaMalloc\n");checkCudaErrors(cudaMemcpyAsync(plan[i].A##_d, plan[i].A##_h, (d)*sizeof(float),cudaMemcpyHostToDevice, plan[i].stream));printf("Pasa cudaMemcpyAsync\n");
#define TODEV(A,d) cudaMalloc((void**)&plan[i].A##_d,(d)*sizeof(float));cudaMemcpyAsync(plan[i].A##_d, plan[i].A##_h, (d)*sizeof(float),cudaMemcpyHostToDevice, plan[i].stream)
#define TODEV3(A) TODEV(A,plan[i].d3)
#define TODEV2(A) TODEV(A,plan[i].d2)

// To host, from device, through pinned memory
//#define FROMDEV(A,d1,d2,s1,s2) checkCudaErrors(cudaMemcpyAsync(plan[i].A##_h,plan[i].A##_d,(d1)*sizeof(float),cudaMemcpyDeviceToHost,plan[i].stream));memcpy(&A[s1], &plan[i].A##_h[(s2)], (d2)*sizeof(float));
//#define FROMDEV(A,d1,d2,s1,s2) cudaMemcpyAsync(&plan[i].A##_h, &plan[i].A##_d, (d1) *sizeof(float), cudaMemcpyDeviceToHost, plan[i].stream);memcpy(&A[s1], &plan[i].A##_h[(s2)], (d2)*sizeof(float));
//#define FROMDEV3(A) FROMDEV(A,plan[i].d3,plan[i].d3_,plan[i].s3_,plan[i].delta)
//#define FROMDEV2(A) FROMDEV(A,plan[i].d2,plan[i].d2_,plan[i].s2_,plan[i].delta)

#define CLNUP(A) cudaFree(plan[i].A##_d)

// Max number of GPUs used
#define MAX_GPU_COUNT 32

/*float * allocAndCopyCuda(float * var, int size, startIndex) {
 float *var2;
 cudaMalloc((void**) &var2, (size) * sizeof(float));
 cudaMemcpy(var2, &var[startIndex], (size) * sizeof(float),
 cudaMemcpyHostToDevice);

 return var2;
 }*/
extern __global__ void horizontal_diffusion_kernel(int ids, int ide, int jds, int jde, int kds,
		int kde, int ims, int ime, int jms, int jme, int kms, int kme, int its, int ite, int jts,
		int jte, int kts, int kte, int cf_specified, int cf_nested, int cf_open_xs, int cf_open_xe,
		int cf_open_ys, int cf_open_ye, int cf_periodic_x, int cf_polar, char name, float *field,
		float *tendency, float *mu, float *msfux, float *msfuy, float *msfvx, float *msfvx_inv,
		float *msfvy, float *msftx, float *msfty, float khdif, float *xkmhd, float rdx, float rdy,
		int sub_js, int sub_je, int data_js, int data_je);

extern "C" {

int horizontal_diffusion_host(int *ids, int *ide, int *jds, int *jde, int *kds, int *kde, int *ims,
		int *ime, int *jms, int *jme, int *kms, int *kme, int *its, int *ite, int *jts, int *jte,
		int *kts, int *kte, int *cf_specified, int *cf_nested, int *cf_open_xs, int *cf_open_xe,
		int *cf_open_ys, int *cf_open_ye, int *cf_periodic_x, int *cf_polar, char *name,
		float *field, float *tendency, float *mu, float *msfux, float *msfuy, float *msfvx,
		float *msfvx_inv, float *msfvy, float *msftx, float *msfty, float *khdif, float *xkmhd,
		float *rdx, float *rdy) {

	int i; // Index
	int GPU_N; // Number of GPUs

	// Timing data
	cudaEvent_t tS0, tE0;
	cudaEventCreate(&tS0);
	cudaEventCreate(&tE0);
	float timer = 0.0f;

	// Original dimensions (Not considering sub-division in j dimension) (For indexing macros)
	const int ix = *ime - *ims + 1;
	const int kx = *kme - *kms + 1;

	printf("Input name: %c\n", *name);
	printf("Tendency first element: %f\n", tendency[P3(6,0,6)]);

	printf("Dimensions:\n");
	printf("ids,ide,jds,jde,kds,kde: %4d,%4d,%4d,%4d,%4d,%4d\n", *ids, *ide, *jds, *jde, *kds,
			*kde);
	printf("ims,ime,jms,jme,kms,kme: %4d,%4d,%4d,%4d,%4d,%4d\n", *ims, *ime, *jms, *jme, *kms,
			*kme);
	printf("its,ite,jts,jte,kts,kte: %4d,%4d,%4d,%4d,%4d,%4d\n", *its, *ite, *jts, *jte, *kts,
			*kte);

	// Solver configuration
	// This structure has the basic I/O data for each calculation of a kernel
	TGPUplan plan[MAX_GPU_COUNT];

	// Get the number of available devices in the node
	// The limit is MAX_GPU_COUNT
	checkCudaErrors(cudaGetDeviceCount(&GPU_N));
	if (GPU_N > MAX_GPU_COUNT) {
		GPU_N = MAX_GPU_COUNT;
	}
	GPU_N = 2; //TODO
	printf("CUDA-capable device count: %i\n", GPU_N);

	// Subdivide workload magnitude
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
		printf("Subdivision of proc (GPU %d) in j dim: (%4d,%4d) (%d)\n", i, plan[i].sub_js,
				plan[i].sub_je, plan[i].sub_dim_j);

		// Sub-region data indices
		if (plan[i].sub_js - 1 < (*ims))
			plan[i].data_js = (*ims);
		else
			plan[i].data_js = plan[i].sub_js - 1;

		if (plan[i].sub_je + 1 > (*jme))
			plan[i].data_je = (*jme);
		else
			plan[i].data_je = plan[i].sub_je + 1;
		printf("Subdivision of data (GPU %d) in j dim: (%4d,%4d) (%d)\n", i, plan[i].data_js,
				plan[i].data_je, (plan[i].data_je - plan[i].data_js + 1));
	}

	//startTimer(1);
	cudaEventRecord(tS0, NULL);

	//Create streams for issuing GPU command asynchronously and allocate memory
	//(GPU and System page-locked/pinned)
	for (i = 0; i < GPU_N; i++) {
		//checkCudaErrors(cudaSetDevice(i)); //TODO
		checkCudaErrors(cudaSetDevice(0));
		checkCudaErrors(cudaStreamCreate(&plan[i].stream));
		// Allocate memory
		// Indices and dimensions for transference
		plan[i].s3 = J(plan[i].data_js) * ((*ime - *ims + 1) * (*kme - *kms + 1));
		plan[i].s2 = J(plan[i].data_js) * ((*ime - *ims + 1));
		plan[i].d3 = (*ime - *ims + 1) * (*kme - *kms + 1)
				* (plan[i].data_je - plan[i].data_js + 1);
		plan[i].d2 = (*ime - *ims + 1) * (plan[i].data_je - plan[i].data_js + 1);

		//Transfer macros to host
		TOHOST3(field);
		TOHOST3(tendency);
		TOHOST3(xkmhd);
		TOHOST2(mu);
		TOHOST2(msfux);
		TOHOST2(msfuy);
		TOHOST2(msfvx);
		TOHOST2(msfvx_inv);
		TOHOST2(msfvy);
		TOHOST2(msftx);
		TOHOST2(msfty);

		// Transfer macros to device
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

		printf("Computing with GPU %d/%d...\n", i, GPU_N);

		int remx, remy;
		remx = (*ime - *ims + 1) % XXX != 0 ? 1 : 0;
		remy = (plan[i].sub_je - plan[i].sub_js + 1) % YYY != 0 ? 1 : 0;
		dim3 dimBlock(XXX, YYY);
		dim3 dimGrid(((*ime - *ims + 1) / XXX) + remx,
				((plan[i].sub_je - plan[i].sub_js + 1) / YYY) + remy);

		printf("Call to kernel: block dims (%4d,%4d)\n", dimBlock.x, dimBlock.y);
		printf("Call to kernel: grid  dims (%4d,%4d)\n", dimGrid.x, dimGrid.y);

		horizontal_diffusion_kernel<<<dimGrid, dimBlock, 0, plan[i].stream>>>(
				*ids, *ide, *jds, *jde, *kds, *kde, *ims, *ime, *jms,
				*jme, *kms, *kme, *its, *ite, *jts, *jte, *kts, *kte, *cf_specified,
				*cf_nested, *cf_open_xs, *cf_open_xe, *cf_open_ys, *cf_open_ye,
				*cf_periodic_x, *cf_polar, *name, plan[i].field_d, plan[i].tendency_d, plan[i].mu_d,
				plan[i].msfux_d, plan[i].msfuy_d, plan[i].msfvx_d, plan[i].msfvx_inv_d, plan[i].msfvy_d, plan[i].msftx_d,
				plan[i].msfty_d, *khdif, plan[i].xkmhd_d, *rdx, *rdy, plan[i].sub_js,
				plan[i].sub_je, plan[i].data_js, plan[i].data_je);
		getLastCudaError("<Kernel execution failed>");

		// Indices for returning data
		plan[i].s3_ = J(plan[i].sub_js) * ((*ime - *ims + 1) * (*kme - *kms + 1));
		plan[i].d3_ = (*ime - *ims + 1) * (*kme - *kms + 1) * (plan[i].sub_je - plan[i].sub_js + 1);
		plan[i].delta = (plan[i].sub_js - plan[i].data_js) * (*ime - *ims + 1) * (*kme - *kms + 1);

		//Read back GPU results to pinned memory
		cudaMemcpyAsync(plan[i].tendency_h, plan[i].tendency_d, (plan[i].d3) * sizeof(float),
				cudaMemcpyDeviceToHost, plan[i].stream);
	}

	//Synchronize GPU and shutdown
	for (i = 0; i < GPU_N; i++) {
		//Set device
		//checkCudaErrors(cudaSetDevice(i));
		//cudaSetDevice(i);
		cudaSetDevice(0); //TODO

		//Wait for all operations to finish
		cudaStreamSynchronize(plan[i].stream);

		// Read back GPU result to Host memory from pinned memory
		memcpy(&tendency[plan[i].s3_], &plan[i].tendency_h[(plan[i].delta)],
				(plan[i].d3_) * sizeof(float));

		// Clean memory
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

		//Shut down this GPU
		//checkCudaErrors(cudaStreamDestroy(plan[i].stream));
		cudaStreamDestroy(plan[i].stream);
	}

	cudaEventRecord(tE0, NULL);
	cudaEventSynchronize(tE0);
	cudaEventElapsedTime(&timer, tS0, tE0);
	printf("Runtime of kernel over %d GPUS using streams: %.3f msec\n", GPU_N, timer);

	return 0;
}

}
