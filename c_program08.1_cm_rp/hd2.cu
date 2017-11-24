#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cublas.h>
#include "hd_block_size.h"
#include "spt2.h"

//#include <stdio.h>
//#include "hd_block_size.h"

#define TODEV(A,s) float *A##_d;cudaMalloc((void**)&A##_d,((s))*sizeof(float));cudaMemcpy(A##_d,A,(s)*sizeof(float),cudaMemcpyHostToDevice);
#define FROMDEV(A,s) cudaMemcpy(A,A##_d,(s)*sizeof(float),cudaMemcpyDeviceToHost);
#define CLNUP(A) cudaFree(A##_d)

#define TODEV3(A) TODEV(A,d3)
#define TODEV2(A) TODEV(A,d2)
#define FROMDEV3(A) FROMDEV(A,d3)
#define FROMDEV2(A) FROMDEV(A,d2)

extern "C" int gethostname(char * name, size_t len);

//extern __device__ __constant__ float rdx_;
//extern __device__ __constant__ float rdy_;
__constant__ float rdx_c[1];
__constant__ float rdy_c[1];

__global__ void horizontal_diffusion_gpu(int ids, int ide, int jds, int jde,
		int kds, int kde, int ims, int ime, int jms, int jme, int kms, int kme,
		int its, int ite, int jts, int jte, int kts, int kte, int cf_specified,
		int cf_nested, int cf_open_xs, int cf_open_xe, int cf_open_ys,
		int cf_open_ye, int cf_periodic_x, int cf_polar, char name,
		float * __restrict__ field, float * __restrict__ tendency,
		float * __restrict__ mu, float * __restrict__ msfux,
		float * __restrict__ msfuy, float * __restrict__ msfvx,
		float * __restrict__ msfvx_inv, float * __restrict__ msfvy,
		float * __restrict__ msftx, float * __restrict__ msfty, float khdif,
		float * __restrict__ xkmhd, float rdx, float rdy) {

	// Local variables
	//int i, j, k, ktf;
	int k, ktf;
	int i_start, i_end, j_start, j_end;
	float mrdx, mkrdxm, mkrdxp, mrdy, mkrdym, mkrdyp;
	int specified = 0;

	// Vars in shared memory
	__shared__ float mu_s[((XXX + 2) * (YYY + 2))];
	__shared__ float msfux_s[((XXX + 2) * (YYY + 2))];
	__shared__ float msfuy_s[((XXX + 2) * (YYY + 2))];
	__shared__ float msftx_s[((XXX + 2) * (YYY + 2))];
	__shared__ float msfty_s[((XXX + 2) * (YYY + 2))];

	if (cf_specified || cf_nested) {
		specified = 1;
	}

	ktf = min(kte, kde - 1);

	if (name == 'u') {
		i_start = its;
		i_end = ite;
		j_start = jts;
		j_end = min(jte, jde - 1);

		if (cf_open_xs || specified)
			i_start = max(ids + 1, its);
		if (cf_open_xe || specified)
			i_end = min(ide - 1, ite);
		if (cf_open_ys || specified)
			j_start = max(jds + 1, jts);
		if (cf_open_ye || specified)
			j_end = min(jde - 2, jte);
		if (cf_periodic_x)
			i_start = its;
		if (cf_periodic_x)
			i_end = ite;

		// Check bounds before calculations!
		if (!(ii >= i_start + 4 && ij <= i_end + 4 && ij >= j_start + 4
				&& ij <= j_end + 4)) {
			return;
		}

		// Redundant copy to shared memory
		mu_s[S2(ti-1,tj-1)] = mu[P2(ti-1,tj-1)];
		mu_s[S2(ti+1,tj-1)] = mu[P2(ti+1,tj-1)];
		mu_s[S2(ti-1,tj+1)] = mu[P2(ti-1,tj+1)];
		mu_s[S2(ti+1,tj+1)] = mu[P2(ti+1,tj+1)];

		msfux_s[S2(ti-1,tj-1)] = msfux[P2(ti-1,tj-1)];
		msfux_s[S2(ti+1,tj-1)] = msfux[P2(ti+1,tj-1)];
		msfux_s[S2(ti-1,tj+1)] = msfux[P2(ti-1,tj+1)];
		msfux_s[S2(ti+1,tj+1)] = msfux[P2(ti+1,tj+1)];

		msfuy_s[S2(ti-1,tj-1)] = msfuy[P2(ti-1,tj-1)];
		msfuy_s[S2(ti+1,tj-1)] = msfuy[P2(ti+1,tj-1)];
		msfuy_s[S2(ti-1,tj+1)] = msfuy[P2(ti-1,tj+1)];
		msfuy_s[S2(ti+1,tj+1)] = msfuy[P2(ti+1,tj+1)];

		msftx_s[S2(ti-1,tj-1)] = msftx[P2(ti-1,tj-1)];
		msftx_s[S2(ti+1,tj-1)] = msftx[P2(ti+1,tj-1)];
		msftx_s[S2(ti-1,tj+1)] = msftx[P2(ti-1,tj+1)];
		msftx_s[S2(ti+1,tj+1)] = msftx[P2(ti+1,tj+1)];

		msfty_s[S2(ti-1,tj-1)] = msfty[P2(ti-1,tj-1)];
		msfty_s[S2(ti+1,tj-1)] = msfty[P2(ti+1,tj-1)];
		msfty_s[S2(ti-1,tj+1)] = msfty[P2(ti-1,tj+1)];
		msfty_s[S2(ti+1,tj+1)] = msfty[P2(ti+1,tj+1)];

		__syncthreads();

		//printf("rdx: %f, rdy: %f \n", rdx_c[0], rdy_c[0]);

		for (k = kts - 1; k <= ktf - 1; k++) {
			// The interior is grad: (m_x*d/dx), the exterior is div: (m_x*m_y*d/dx(/m_y))
			// setting up different averagings of m^2 partial d/dX and m^2 partial d/dY

			mkrdxm = (msftx_s[S2(ti-1,tj)] / msfty_s[S2(ti-1,tj)])
					* mu_s[S2(ti-1,tj)] * xkmhd[P3(ti-1,k,tj)] * (rdx_c[0]);
			mkrdxp = (msftx_s[S2(ti,tj)] / msfty_s[S2(ti,tj)]) * mu_s[S2(ti,tj)]
					* xkmhd[P3(ti,k,tj)] * (rdx_c[0]);
			mrdx = msfux_s[S2(ti,tj)] * msfuy_s[S2(ti,tj)] * (rdx_c[0]);

			mkrdym = ((msfuy_s[S2(ti,tj)] + msfuy_s[S2(ti,tj-1)])
					/ (msfux_s[S2(ti,tj)] + msfux_s[S2(ti,tj-1)])) * 0.25
					* (mu_s[S2(ti,tj)] + mu_s[S2(ti,tj-1)] + mu_s[S2(ti-1,tj-1)]
							+ mu_s[S2(ti-1,tj)]) * 0.25
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj-1)]
							+ xkmhd[P3(ti-1,k,tj-1)] + xkmhd[P3(ti-1,k,tj)])
					* (rdy_c[0]);
			mkrdyp = ((msfuy_s[S2(ti,tj)] + msfuy_s[S2(ti,tj+1)])
					/ (msfux_s[S2(ti,tj)] + msfux_s[S2(ti,tj+1)])) * 0.25
					* (mu_s[S2(ti,tj)] + mu_s[S2(ti,tj+1)] + mu_s[S2(ti-1,tj+1)]
							+ mu_s[S2(ti-1,tj)]) * 0.25
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj+1)]
							+ xkmhd[P3(ti-1,k,tj+1)] + xkmhd[P3(ti-1,k,tj)])
					* (rdy_c[0]);

			// need to do four-corners (t) for diffusion coefficient as there are
			// no values at u,v points
			// msfuy - has to be y as part of d/dY
			//         has to be u as we're at a u point
			mrdy = msfux_s[S2(ti,tj)] * msfuy_s[S2(ti,tj)] * (rdy_c[0]);

			// correctly averaged version of rho~ * m^2 *
			//    [partial d/dX(partial du^/dX) + partial d/dY(partial du^/dY)]
			/*tendency[P3(ti,k,tj)]=tendency[P3(ti,k,tj)]+(
			 mrdx*(mkrdxp*(field[P3(ti+1,k,tj)]-field[P3(ti,k,tj)])
			 -mkrdxm*(field[P3(ti,k,tj)]-field[P3(ti-1,k,tj)]))
			 +mrdy*(mkrdyp*(field[P3(ti,k,tj+1)]-field[P3(ti,k,tj)])
			 -mkrdym*(field[P3(ti,k,tj)]-field[P3(ti,k,tj-1)])));*/
			tendency[P3(ti,k,tj)] =
					tendency[P3(ti,k,tj)]
							+ (mrdx
									* (mkrdxp
											* (field[P3(ti+1,k,tj)]
													- field[P3(ti,k,tj)])
											- mkrdxm
													* (field[P3(ti,k,tj)]
															- field[P3(ti-1,k,tj)]))
									+ mrdy
											* (mkrdyp
													* (field[P3(ti,k,tj+1)]
															- field[P3(ti,k,tj)])
													- mkrdym
															* (field[P3(ti,k,tj)]
																	- field[P3(ti,k,tj-1)])));
		}
	} else if (name == 'v') {

		i_start = its;
		i_end = min(ite, ide - 1);
		j_start = jts;
		j_end = jte;

		if (cf_open_xs || specified)
			i_start = max(ids + 1, its);
		if (cf_open_xe || specified)
			i_end = min(ide - 2, ite);
		if (cf_open_ys || specified)
			j_start = max(jds + 1, jts);
		if (cf_open_ye || specified)
			j_end = min(jde - 1, jte);
		if (cf_periodic_x)
			i_start = its;
		if (cf_periodic_x)
			i_end = min(ite, ide - 1);
		if (cf_polar)
			j_start = max(jds + 1, jts);
		if (cf_polar)
			j_end = min(jde - 1, jte);

		// Check bounds before calculations!
		if (!(ii >= i_start - 1 && ii <= i_end - 1 && ij >= j_start - 1
				&& ij <= j_end - 1)) {
			return;
		}

		for (k = kts - 1; k <= ktf - 1; k++) {

			mkrdxm = ((msfvx[P2(ti,tj)] + msfvx[P2(ti-1,tj)])
					/ (msfvy[P2(ti,tj)] + msfvy[P2(ti-1,tj)])) * 0.25
					* (mu[P2(ti,tj)] + mu[P2(ti,tj-1)] + mu[P2(ti-1,tj-1)]
							+ mu[P2(ti-1,tj)]) * 0.25
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj-1)]
							+ xkmhd[P3(ti-1,k,tj-1)] + xkmhd[P3(ti-1,k,tj)])
					* rdx;
			mkrdxp = ((msfvx[P2(ti,tj)] + msfvx[P2(ti+1,tj)])
					/ (msfvy[P2(ti,tj)] + msfvy[P2(ti+1,tj)])) * 0.25
					* (mu[P2(ti,tj)] + mu[P2(ti,tj-1)] + mu[P2(ti+1,tj-1)]
							+ mu[P2(ti+1,tj)]) * 0.25
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj-1)]
							+ xkmhd[P3(ti+1,k,tj-1)] + xkmhd[P3(ti+1,k,tj)])
					* rdx;
			mrdx = msfvx[P2(ti,tj)] * msfvy[P2(ti,tj)] * rdx;
			mkrdym = (msfty[P2(ti,tj-1)] / msftx[P2(ti,tj-1)])
					* xkmhd[P3(ti,k,tj-1)] * rdy;
			mkrdyp = (msfty[P2(ti,tj)] / msftx[P2(ti,tj)]) * xkmhd[P3(ti,k,tj)]
					* rdy;
			mrdy = msfvx[P2(ti,tj)] * msfvy[P2(ti,tj)] * rdy;

			tendency[P3(ti,k,tj)] =
					tendency[P3(ti,k,tj)]
							+ (mrdx
									* (mkrdxp
											* (field[P3(ti+1,k,tj)]
													- field[P3(ti,k,tj)])
											- mkrdxm
													* (field[P3(ti,k,tj)]
															- field[P3(ti-1,k,tj)]))
									+ mrdy
											* (mkrdyp
													* (field[P3(ti,k,tj+1)]
															- field[P3(ti,k,tj)])
													- mkrdym
															* (field[P3(ti,k,tj)]
																	- field[P3(ti,k,tj-1)])));
		}
	} else if (name == 'w') {

		i_start = its;
		i_end = min(ite, ide - 1);
		j_start = jts;
		j_end = min(jte, jde - 1);

		if (cf_open_xs || specified)
			i_start = max(ids + 1, its);
		if (cf_open_xe || specified)
			i_end = min(ide - 2, ite);
		if (cf_open_ys || specified)
			j_start = max(jds + 1, jts);
		if (cf_open_ye || specified)
			j_end = min(jde - 2, jte);
		if (cf_periodic_x)
			i_start = its;
		if (cf_periodic_x)
			i_end = min(ite, ide - 1);

		// Check bounds before calculations!
		if (!(ii >= i_start - 1 && ii <= i_end - 1 && ij >= j_start - 1
				&& ij <= j_end - 1)) {
			return;
		}

		for (k = kts - 1; k <= ktf - 1; k++) {

			mkrdxm = (msfux[P2(ti,tj)] / msfuy[P2(ti,tj)]) * 0.25
					* (mu[P2(ti,tj)] + mu[P2(ti-1,tj)] + mu[P2(ti,tj)]
							+ mu[P2(ti-1,tj)]) * 0.25
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti-1,k,tj)]
							+ xkmhd[P3(ti,k-1,tj)] + xkmhd[P3(ti-1,k-1,tj)])
					* rdx;
			mkrdxp = (msfux[P2(ti+1,tj)] / msfuy[P2(ti+1,tj)]) * 0.25
					* (mu[P2(ti+1,tj)] + mu[P2(ti,tj)] + mu[P2(ti+1,tj)]
							+ mu[P2(ti,tj)]) * 0.25
					* (xkmhd[P3(ti+1,k,tj)] + xkmhd[P3(ti,k,tj)]
							+ xkmhd[P3(ti+1,k-1,tj)] + xkmhd[P3(ti,k-1,tj)])
					* rdx;
			mrdx = msftx[P2(ti,tj)] * msfty[P2(ti,tj)] * rdx;
			//         mkrdym=(msfvy[P2(ti,tj)]/msfvx[P2(ti,tj)])*
			mkrdym = (msfvy[P2(ti,tj)] * msfvx_inv[P2(ti,tj)]) * 0.25
					* (mu[P2(ti,tj)] + mu[P2(ti,tj-1)] + mu[P2(ti,tj)]
							+ mu[P2(ti,tj-1)]) * 0.25
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj-1)]
							+ xkmhd[P3(ti,k-1,tj)] + xkmhd[P3(ti,k-1,tj-1)])
					* rdy;
			//         mkrdyp=(msfvy[P2(ti,tj+1)]/msfvx[P2(ti,tj+1)])*
			mkrdyp = (msfvy[P2(ti,tj+1)] * msfvx_inv[P2(ti,tj+1)]) * 0.25
					* (mu[P2(ti,tj+1)] + mu[P2(ti,tj)] + mu[P2(ti,tj+1)]
							+ mu[P2(ti,tj)]) * 0.25
					* (xkmhd[P3(ti,k,tj+1)] + xkmhd[P3(ti,k,tj)]
							+ xkmhd[P3(ti,k-1,tj+1)] + xkmhd[P3(ti,k-1,tj)])
					* rdy;
			mrdy = msftx[P2(ti,tj)] * msfty[P2(ti,tj)] * rdy;

			tendency[P3(ti,k,tj)] =
					tendency[P3(ti,k,tj)]
							+ (mrdx
									* (mkrdxp
											* (field[P3(ti+1,k,tj)]
													- field[P3(ti,k,tj)])
											- mkrdxm
													* (field[P3(ti,k,tj)]
															- field[P3(ti-1,k,tj)]))
									+ mrdy
											* (mkrdyp
													* (field[P3(ti,k,tj+1)]
															- field[P3(ti,k,tj)])
													- mkrdym
															* (field[P3(ti,k,tj)]
																	- field[P3(ti,k,tj-1)])));
		}
	} else {

		i_start = its;
		i_end = min(ite, ide - 1);
		j_start = jts;
		j_end = min(jte, jde - 1);

		if (cf_open_xs || specified)
			i_start = max(ids + 1, its);
		if (cf_open_xe || specified)
			i_end = min(ide - 2, ite);
		if (cf_open_ys || specified)
			j_start = max(jds + 1, jts);
		if (cf_open_ye || specified)
			j_end = min(jde - 2, jte);
		if (cf_periodic_x)
			i_start = its;
		if (cf_periodic_x)
			i_end = min(ite, ide - 1);

		for (k = kts; k <= ktf; k++) {

			mkrdxm = (msfux[P2(ti,tj)] / msfuy[P2(ti,tj)]) * 0.5
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti-1,k,tj)]) * 0.5
					* (mu[P2(ti,tj)] + mu[P2(ti-1,tj)]) * rdx;
			mkrdxp = (msfux[P2(ti+1,tj)] / msfuy[P2(ti+1,tj)]) * 0.5
					* (xkmhd[P3(ti+1,k,tj)] + xkmhd[P3(ti,k,tj)]) * 0.5
					* (mu[P2(ti+1,tj)] + mu[P2(ti,tj)]) * rdx;
			mrdx = msftx[P2(ti,tj)] * msfty[P2(ti,tj)] * rdx;
			//         mkrdym=(msfvy[P2(ti,tj)]/msfvx[P2(ti,tj)])*0.5*(xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti,k,tj-1)])*0.5*(mu[P2(ti,tj)]+mu[P2(ti,tj-1)])*rdy
			mkrdym = (msfvy[P2(ti,tj)] * msfvx_inv[P2(ti,tj)]) * 0.5
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj-1)]) * 0.5
					* (mu[P2(ti,tj)] + mu[P2(ti,tj-1)]) * rdy;
			//         mkrdyp=(msfvy[P2(ti,tj+1)]/msfvx[P2(ti,tj+1)])*0.5*(xkmhd[P3(ti,k,tj+1)]+xkmhd[P3(ti,k,tj)])*0.5*(mu[P2(ti,tj+1)]+mu[P2(ti,tj)])*rdy
			mkrdyp = (msfvy[P2(ti,tj+1)] * msfvx_inv[P2(ti,tj+1)]) * 0.5
					* (xkmhd[P3(ti,k,tj+1)] + xkmhd[P3(ti,k,tj)]) * 0.5
					* (mu[P2(ti,tj+1)] + mu[P2(ti,tj)]) * rdy;
			mrdy = msftx[P2(ti,tj)] * msfty[P2(ti,tj)] * rdy;

			tendency[P3(ti,k,tj)] =
					tendency[P3(ti,k,tj)]
							+ (mrdx
									* (mkrdxp
											* (field[P3(ti+1,k,tj)]
													- field[P3(ti,k,tj)])
											- mkrdxm
													* (field[P3(ti,k,tj)]
															- field[P3(ti-1,k,tj)]))
									+ mrdy
											* (mkrdyp
													* (field[P3(ti,k,tj+1)]
															- field[P3(ti,k,tj)])
													- mkrdym
															* (field[P3(ti,k,tj)]
																	- field[P3(ti,k,tj-1)])));
		}
	}

}

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

	dim3 dimBlock(XXX, YYY);
	dim3 dimGrid(((*ime - *ims + 1) / XXX) + remx,
			((*jme - *jms + 1) / YYY) + remy);

	printf("Call to kernel: block dims %d %d\n", dimBlock.x, dimBlock.y);
	printf("Call to kernel: grid  dims %d %d\n", dimGrid.x, dimGrid.y);
	printf("Calling kernel \n");

	cudaEventRecord(tS1, NULL);

	// Set constant memory
	//cudaMemcpyToSymbol(rdx_, rdx, sizeof(float));
	//cudaMemcpyToSymbol(rdy_, rdy, sizeof(float));

	cudaMemcpyToSymbol(rdx_c, rdx, sizeof(float));
	cudaMemcpyToSymbol(rdy_c, rdy, sizeof(float));

	// Changing cache configuration
	cudaFuncSetCacheConfig(horizontal_diffusion_gpu, cudaFuncCachePreferL1);

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

}
