#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>
#include "spt1.h"
#include "spt2.h"
#include "multiGPU.h"

__global__ void horizontal_diffusion_kernel(int ids, int ide, int jds, int jde, int kds, int kde,
		int ims, int ime, int jms, int jme, int kms, int kme, int its, int ite, int jts, int jte,
		int kts, int kte, int cf_specified, int cf_nested, int cf_open_xs, int cf_open_xe,
		int cf_open_ys, int cf_open_ye, int cf_periodic_x, int cf_polar, char name, float *field,
		float *tendency, float *mu, float *msfux, float *msfuy, float *msfvx, float *msfvx_inv,
		float *msfvy, float *msftx, float *msfty, float khdif, float *xkmhd, float rdx, float rdy,
		int sub_js, int sub_je, int data_js, int data_je) {

	// Dimensions of the 3d grid
	const int ix = ime - ims + 1;
	const int jx = data_je - data_js + 1;
	const int kx = kme - kms + 1;

	// Local variables
	int i, j, k, ktf;
	int i_start, i_end, j_start, j_end;
	float mrdx, mkrdxm, mkrdxp, mrdy, mkrdym, mkrdyp;
	int specified = 0;

	if (cf_specified || cf_nested) {
		specified = 1;
	}

	ktf = MIN(kte,kde-1);

	// Thread coordinates
	i = bi * bx + ti;
	j = bj * by + tj;

	if (name == 'u') {
		i_start = its;
		i_end = ite;
		j_start = jts;
		j_end = MIN(jte,jde-1);

		if (cf_open_xs || specified)
			i_start = MAX(ids+1,its);
		if (cf_open_xe || specified)
			i_end = MIN(ide-1,ite);
		if (cf_open_ys || specified)
			j_start = MAX(jds+1,jts);
		if (cf_open_ye || specified)
			j_end = MIN(jde-2,jte);
		if (cf_periodic_x)
			i_start = its;
		if (cf_periodic_x)
			i_end = ite;

		j_start = MAX(j_start, sub_js); // In Fortran type index
		j_end = MIN(j_end, sub_je); // In Fortran type index

		// Check bounds before calculations!
		if (!(i >= I(i_start) && i <= I(i_end) && j >= j_start - data_js && j <= j_end - data_js)) {
			return;
		}

		for (k = K(kts); k <= K(ktf); k++) {

			// The interior is grad: (m_x*d/dx), the exterior is div: (m_x*m_y*d/dx(/m_y))
			// setting up different averagings of m^2 partial d/dX and m^2 partial d/dY

			mkrdxm = (msftx[Q2(ti - 1, tj)] / msfty[Q2(ti - 1, tj)]) * mu[Q2(ti - 1, tj)]
					* xkmhd[Q3(ti - 1, k, tj)] * (rdx);
			mkrdxp = (msftx[Q2(ti, tj)] / msfty[Q2(ti, tj)]) * mu[Q2(ti, tj)] * xkmhd[Q3(ti, k, tj)]
					* (rdx);
			mrdx = msfux[Q2(ti, tj)] * msfuy[Q2(ti, tj)] * (rdx);
			mkrdym = ((msfuy[Q2(ti, tj)] + msfuy[Q2(ti, tj - 1)])
					/ (msfux[Q2(ti, tj)] + msfux[Q2(ti, tj - 1)])) * 0.25
					* (mu[Q2(ti, tj)] + mu[Q2(ti, tj - 1)] + mu[Q2(ti - 1, tj - 1)]
							+ mu[Q2(ti - 1, tj)]) * 0.25
					* (xkmhd[Q3(ti, k, tj)] + xkmhd[Q3(ti, k, tj - 1)]
							+ xkmhd[Q3(ti - 1, k, tj - 1)] + xkmhd[Q3(ti - 1, k, tj)]) * (rdy);
			mkrdyp = ((msfuy[Q2(ti, tj)] + msfuy[Q2(ti, tj + 1)])
					/ (msfux[Q2(ti, tj)] + msfux[Q2(ti, tj + 1)])) * 0.25
					* (mu[Q2(ti, tj)] + mu[Q2(ti, tj + 1)] + mu[Q2(ti - 1, tj + 1)]
							+ mu[Q2(ti - 1, tj)]) * 0.25
					* (xkmhd[Q3(ti, k, tj)] + xkmhd[Q3(ti, k, tj + 1)]
							+ xkmhd[Q3(ti - 1, k, tj + 1)] + xkmhd[Q3(ti - 1, k, tj)]) * (rdy);

			// need to do four-corners (t) for diffusion coefficient as there are
			// no values at u,v points
			// msfuy - has to be y as part of d/dY
			//         has to be u as we're at a u point
			mrdy = msfux[Q2(ti, tj)] * msfuy[Q2(ti, tj)] * (rdy);

			// correctly averaged version of rho~ * m^2 *
			//    [partial d/dX(partial du^/dX) + partial d/dY(partial du^/dY)]

			tendency[Q3(ti, k, tj)] = tendency[Q3(ti, k, tj)]
					+ (mrdx
							* (mkrdxp * (field[Q3(ti + 1, k, tj)] - field[Q3(ti, k, tj)])
									- mkrdxm * (field[Q3(ti, k, tj)] - field[Q3(ti - 1, k, tj)]))
							+ mrdy
									* (mkrdyp * (field[Q3(ti, k, tj + 1)] - field[Q3(ti, k, tj)])
											- mkrdym
													* (field[Q3(ti, k, tj)]
															- field[Q3(ti, k, tj - 1)])));
		}
	}

}
