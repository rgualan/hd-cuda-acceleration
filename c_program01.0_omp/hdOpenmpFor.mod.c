#ifdef _POMP
#  undef _POMP
#endif
#define _POMP 200110

#include "hdOpenmpFor.c.opari.inc"
#line 1 "hdOpenmpFor.c"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "spt2.h"

void horizontal_diffusion_c_(int *ids, int *ide, int *jds, int *jde, int *kds,
		int *kde, int *ims, int *ime, int *jms, int *jme, int *kms, int *kme,
		int *its, int *ite, int *jts, int *jte, int *kts, int *kte,
		int *cf_specified, int *cf_nested, int *cf_open_xs, int *cf_open_xe,
		int *cf_open_ys, int *cf_open_ye, int *cf_periodic_x, int *cf_polar,
		char *name, float *field, float *tendency, float *mu, float *msfux,
		float *msfuy, float *msfvx, float *msfvx_inv, float *msfvy,
		float *msftx, float *msfty, float *khdif, float *xkmhd, float *rdx,
		float *rdy) {

	// Dimensions
	const int ix = *ime - *ims + 1;
	const int jx = *jme - *jms + 1;
	const int kx = *kme - *kms + 1;

	// Local variables
	int i, j, k, ktf;
	int i_start, i_end, j_start, j_end;
	float mrdx, mkrdxm, mkrdxp, mrdy, mkrdym, mkrdyp;
	int specified = 0;

	if (*cf_specified || *cf_nested) {
		specified = 1;
	}

	ktf = MIN(*kte,*kde-1);

	// OpenMP
	printf("OpenMP max threads: %d \n", omp_get_max_threads());

	if (*name == 'u') {
		i_start = *its;
		i_end = *ite;
		j_start = *jts;
		j_end = MIN(*jte,*jde-1);

		if (*cf_open_xs || specified)
			i_start = MAX(*ids+1,*its);
		if (*cf_open_xe || specified)
			i_end = MIN(*ide-1,*ite);
		if (*cf_open_ys || specified)
			j_start = MAX(*jds+1,*jts);
		if (*cf_open_ye || specified)
			j_end = MIN(*jde-2,*jte);
		if (*cf_periodic_x)
			i_start = *its;
		if (*cf_periodic_x)
			i_end = *ite;

		printf("i proc. dim: %d %d (%d)\n", I(i_start), I(i_end), ix);
		printf("j proc. dim: %d %d (%d)\n", J(j_start), J(j_end), jx);
		printf("k proc. dim: %d %d (%d)\n", K(*kts), K(ktf), kx);

		int chunk;

		double start;
		start = omp_get_wtime();

//#pragma omp parallel shared(field, tendency, mu, msfux, msfuy, msfvx, msfvx_inv, msfvy,msftx, msfty, xkmhd) private(i,k,j)
//#pragma omp parallel default(shared) private(j,k,i)
//#pragma omp parallel default(shared) private(j,k,i)
#line 68 "hdOpenmpFor.c"
#pragma omp parallel default(shared) private(j,k,i,chunk, mkrdxm, mkrdxp, mrdx, mkrdym, mkrdyp, mrdy)
		{
			chunk = (J(j_end) - J(j_start) + 1) / omp_get_num_threads();
			//printf("Thread %d running... (%d)\n", omp_get_thread_num(), chunk);
			//printf("Thread %d running... \n", omp_get_thread_num());
//#pragma omp parallel for schedule(static) default(shared) private(j,k,i,chunk, mkrdxm, mkrdxp, mrdx, mkrdym, mkrdyp, mrdy)
#line 74 "hdOpenmpFor.c"
#pragma omp for schedule(static) nowait
			for (j = J(j_start); j <= J(j_end); j++) {
				for (k = K(*kts); k <= K(ktf); k++) {
					for (i = I(i_start); i <= I(i_end); i++) {
						// The interior is grad: (m_x*d/dx), the exterior is div: (m_x*m_y*d/dx(/m_y))
						// setting up different averagings of m^2 partial d/dX and m^2 partial d/dY

						mkrdxm = (msftx[P2(i-1,j)] / msfty[P2(i-1,j)])
								* mu[P2(i-1,j)] * xkmhd[P3(i-1,k,j)] * (*rdx);
						mkrdxp = (msftx[P2(i,j)] / msfty[P2(i,j)]) * mu[P2(i,j)]
								* xkmhd[P3(i,k,j)] * (*rdx);
						mrdx = msfux[P2(i,j)] * msfuy[P2(i,j)] * (*rdx);

						mkrdym = ((msfuy[P2(i,j)] + msfuy[P2(i,j-1)])
								/ (msfux[P2(i,j)] + msfux[P2(i,j-1)])) * 0.25
								* (mu[P2(i,j)] + mu[P2(i,j-1)] + mu[P2(i-1,j-1)]
										+ mu[P2(i-1,j)]) * 0.25
								* (xkmhd[P3(i,k,j)] + xkmhd[P3(i,k,j-1)]
										+ xkmhd[P3(i-1,k,j-1)]
										+ xkmhd[P3(i-1,k,j)]) * (*rdy);
						mkrdyp = ((msfuy[P2(i,j)] + msfuy[P2(i,j+1)])
								/ (msfux[P2(i,j)] + msfux[P2(i,j+1)])) * 0.25
								* (mu[P2(i,j)] + mu[P2(i,j+1)] + mu[P2(i-1,j+1)]
										+ mu[P2(i-1,j)]) * 0.25
								* (xkmhd[P3(i,k,j)] + xkmhd[P3(i,k,j+1)]
										+ xkmhd[P3(i-1,k,j+1)]
										+ xkmhd[P3(i-1,k,j)]) * (*rdy);

						// need to do four-corners (t) for diffusion coefficient as there are
						// no values at u,v points
						// msfuy - has to be y as part of d/dY
						//         has to be u as we're at a u point
						mrdy = msfux[P2(i,j)] * msfuy[P2(i,j)] * (*rdy);

						// correctly averaged version of rho~ * m^2 *
						//    [partial d/dX(partial du^/dX) + partial d/dY(partial du^/dY)]

						tendency[P3(i,k,j)] =
								tendency[P3(i,k,j)]
										+ (mrdx
												* (mkrdxp
														* (field[P3(i+1,k,j)]
																- field[P3(i,k,j)])
														- mkrdxm
																* (field[P3(i,k,j)]
																		- field[P3(i-1,k,j)]))
												+ mrdy
														* (mkrdyp
																* (field[P3(i,k,j+1)]
																		- field[P3(i,k,j)])
																- mkrdym
																		* (field[P3(i,k,j)]
																				- field[P3(i,k,j-1)])));

					}
				}
			}
		}
		printf(
				"Time for running horizontal_diffusion (omp_get_wtime):\t%f ms\n",
				(omp_get_wtime() - start) * 1000);

	} else if (*name == 'v') {

		i_start = *its;
		i_end = MIN(*ite,*ide-1);
		j_start = *jts;
		j_end = *jte;

		if (*cf_open_xs || specified)
			i_start = MAX(*ids+1,*its);
		if (*cf_open_xe || specified)
			i_end = MIN(*ide-2,*ite);
		if (*cf_open_ys || specified)
			j_start = MAX(*jds+1,*jts);
		if (*cf_open_ye || specified)
			j_end = MIN(*jde-1,*jte);
		if (*cf_periodic_x)
			i_start = *its;
		if (*cf_periodic_x)
			i_end = MIN(*ite,*ide-1);
		if (cf_polar)
			j_start = MAX(*jds+1,*jts);
		if (cf_polar)
			j_end = MIN(*jde-1,*jte);

		// Check bounds before calculations!
		for (k = *kts - 1; k <= ktf - 1; k++) {
			for (j = j_start + 4; j <= j_end + 4; j++) {
				for (i = i_start + 4; i <= i_end + 4; i++) {

					mkrdxm = ((msfvx[P2(i,j)] + msfvx[P2(i-1,j)])
							/ (msfvy[P2(i,j)] + msfvy[P2(i-1,j)])) * 0.25
							* (mu[P2(i,j)] + mu[P2(i,j-1)] + mu[P2(i-1,j-1)]
									+ mu[P2(i-1,j)]) * 0.25
							* (xkmhd[P3(i,k,j)] + xkmhd[P3(i,k,j-1)]
									+ xkmhd[P3(i-1,k,j-1)] + xkmhd[P3(i-1,k,j)])
							* (*rdx);
					mkrdxp = ((msfvx[P2(i,j)] + msfvx[P2(i+1,j)])
							/ (msfvy[P2(i,j)] + msfvy[P2(i+1,j)])) * 0.25
							* (mu[P2(i,j)] + mu[P2(i,j-1)] + mu[P2(i+1,j-1)]
									+ mu[P2(i+1,j)]) * 0.25
							* (xkmhd[P3(i,k,j)] + xkmhd[P3(i,k,j-1)]
									+ xkmhd[P3(i+1,k,j-1)] + xkmhd[P3(i+1,k,j)])
							* (*rdx);
					mrdx = msfvx[P2(i,j)] * msfvy[P2(i,j)] * (*rdx);
					mkrdym = (msfty[P2(i,j-1)] / msftx[P2(i,j-1)])
							* xkmhd[P3(i,k,j-1)] * (*rdy);
					mkrdyp = (msfty[P2(i,j)] / msftx[P2(i,j)])
							* xkmhd[P3(i,k,j)] * (*rdy);
					mrdy = msfvx[P2(i,j)] * msfvy[P2(i,j)] * (*rdy);

					tendency[P3(i,k,j)] =
							tendency[P3(i,k,j)]
									+ (mrdx
											* (mkrdxp
													* (field[P3(i+1,k,j)]
															- field[P3(i,k,j)])
													- mkrdxm
															* (field[P3(i,k,j)]
																	- field[P3(i-1,k,j)]))
											+ mrdy
													* (mkrdyp
															* (field[P3(i,k,j+1)]
																	- field[P3(i,k,j)])
															- mkrdym
																	* (field[P3(i,k,j)]
																			- field[P3(i,k,j-1)])));
				}
			}
		}
	} else if (*name == 'w') {

		i_start = *its;
		i_end = MIN(*ite,*ide-1);
		j_start = *jts;
		j_end = MIN(*jte,*jde-1);

		if (*cf_open_xs || specified)
			i_start = MAX(*ids+1,*its);
		if (*cf_open_xe || specified)
			i_end = MIN(*ide-2,*ite);
		if (*cf_open_ys || specified)
			j_start = MAX(*jds+1,*jts);
		if (*cf_open_ye || specified)
			j_end = MIN(*jde-2,*jte);
		if (*cf_periodic_x)
			i_start = *its;
		if (*cf_periodic_x)
			i_end = MIN(*ite,*ide-1);

		// Check bounds before calculations!
		for (k = *kts - 1; k <= ktf - 1; k++) {
			for (j = j_start + 4; j <= j_end + 4; j++) {
				for (i = i_start + 4; i <= i_end + 4; i++) {

					mkrdxm = (msfux[P2(i,j)] / msfuy[P2(i,j)]) * 0.25
							* (mu[P2(i,j)] + mu[P2(i-1,j)] + mu[P2(i,j)]
									+ mu[P2(i-1,j)]) * 0.25
							* (xkmhd[P3(i,k,j)] + xkmhd[P3(i-1,k,j)]
									+ xkmhd[P3(i,k-1,j)] + xkmhd[P3(i-1,k-1,j)])
							* (*rdx);
					mkrdxp = (msfux[P2(i+1,j)] / msfuy[P2(i+1,j)]) * 0.25
							* (mu[P2(i+1,j)] + mu[P2(i,j)] + mu[P2(i+1,j)]
									+ mu[P2(i,j)]) * 0.25
							* (xkmhd[P3(i+1,k,j)] + xkmhd[P3(i,k,j)]
									+ xkmhd[P3(i+1,k-1,j)] + xkmhd[P3(i,k-1,j)])
							* (*rdx);
					mrdx = msftx[P2(i,j)] * msfty[P2(i,j)] * (*rdx);
					//         mkrdym=(msfvy[P2(i,j)]/msfvx[P2(i,j)])*
					mkrdym = (msfvy[P2(i,j)] * msfvx_inv[P2(i,j)]) * 0.25
							* (mu[P2(i,j)] + mu[P2(i,j-1)] + mu[P2(i,j)]
									+ mu[P2(i,j-1)]) * 0.25
							* (xkmhd[P3(i,k,j)] + xkmhd[P3(i,k,j-1)]
									+ xkmhd[P3(i,k-1,j)] + xkmhd[P3(i,k-1,j-1)])
							* (*rdy);
					//         mkrdyp=(msfvy[P2(i,j+1)]/msfvx[P2(i,j+1)])*
					mkrdyp = (msfvy[P2(i,j+1)] * msfvx_inv[P2(i,j+1)]) * 0.25
							* (mu[P2(i,j+1)] + mu[P2(i,j)] + mu[P2(i,j+1)]
									+ mu[P2(i,j)]) * 0.25
							* (xkmhd[P3(i,k,j+1)] + xkmhd[P3(i,k,j)]
									+ xkmhd[P3(i,k-1,j+1)] + xkmhd[P3(i,k-1,j)])
							* (*rdy);
					mrdy = msftx[P2(i,j)] * msfty[P2(i,j)] * (*rdy);

					tendency[P3(i,k,j)] =
							tendency[P3(i,k,j)]
									+ (mrdx
											* (mkrdxp
													* (field[P3(i+1,k,j)]
															- field[P3(i,k,j)])
													- mkrdxm
															* (field[P3(i,k,j)]
																	- field[P3(i-1,k,j)]))
											+ mrdy
													* (mkrdyp
															* (field[P3(i,k,j+1)]
																	- field[P3(i,k,j)])
															- mkrdym
																	* (field[P3(i,k,j)]
																			- field[P3(i,k,j-1)])));
				}
			}
		}
	} else {

		i_start = *its;
		i_end = MIN(*ite,*ide-1);
		j_start = *jts;
		j_end = MIN(*jte,*jde-1);

		if (*cf_open_xs || specified)
			i_start = MAX(*ids+1,*its);
		if (*cf_open_xe || specified)
			i_end = MIN(*ide-2,*ite);
		if (*cf_open_ys || specified)
			j_start = MAX(*jds+1,*jts);
		if (*cf_open_ye || specified)
			j_end = MIN(*jde-2,*jte);
		if (*cf_periodic_x)
			i_start = *its;
		if (*cf_periodic_x)
			i_end = MIN(*ite,*ide-1);

		for (k = *kts - 1; k <= ktf - 1; k++) {
			for (j = j_start + 4; j <= j_end + 4; j++) {
				for (i = i_start + 4; i <= i_end + 4; i++) {

					mkrdxm = (msfux[P2(i,j)] / msfuy[P2(i,j)]) * 0.5
							* (xkmhd[P3(i,k,j)] + xkmhd[P3(i-1,k,j)]) * 0.5
							* (mu[P2(i,j)] + mu[P2(i-1,j)]) * (*rdx);
					mkrdxp = (msfux[P2(i+1,j)] / msfuy[P2(i+1,j)]) * 0.5
							* (xkmhd[P3(i+1,k,j)] + xkmhd[P3(i,k,j)]) * 0.5
							* (mu[P2(i+1,j)] + mu[P2(i,j)]) * (*rdx);
					mrdx = msftx[P2(i,j)] * msfty[P2(i,j)] * (*rdx);
					//         mkrdym=(msfvy[P2(i,j)]/msfvx[P2(i,j)])*0.5*(xkmhd[P3(i,k,j)]+xkmhd[P3(i,k,j-1)])*0.5*(mu[P2(i,j)]+mu[P2(i,j-1)])*rdy
					mkrdym = (msfvy[P2(i,j)] * msfvx_inv[P2(i,j)]) * 0.5
							* (xkmhd[P3(i,k,j)] + xkmhd[P3(i,k,j-1)]) * 0.5
							* (mu[P2(i,j)] + mu[P2(i,j-1)]) * (*rdy);
					//         mkrdyp=(msfvy[P2(i,j+1)]/msfvx[P2(i,j+1)])*0.5*(xkmhd[P3(i,k,j+1)]+xkmhd[P3(i,k,j)])*0.5*(mu[P2(i,j+1)]+mu[P2(i,j)])*rdy
					mkrdyp = (msfvy[P2(i,j+1)] * msfvx_inv[P2(i,j+1)]) * 0.5
							* (xkmhd[P3(i,k,j+1)] + xkmhd[P3(i,k,j)]) * 0.5
							* (mu[P2(i,j+1)] + mu[P2(i,j)]) * (*rdy);
					mrdy = msftx[P2(i,j)] * msfty[P2(i,j)] * (*rdy);

					tendency[P3(i,k,j)] =
							tendency[P3(i,k,j)]
									+ (mrdx
											* (mkrdxp
													* (field[P3(i+1,k,j)]
															- field[P3(i,k,j)])
													- mkrdxm
															* (field[P3(i,k,j)]
																	- field[P3(i-1,k,j)]))
											+ mrdy
													* (mkrdyp
															* (field[P3(i,k,j+1)]
																	- field[P3(i,k,j)])
															- mkrdym
																	* (field[P3(i,k,j)]
																			- field[P3(i,k,j-1)])));
				}
			}
		}
	}

}
