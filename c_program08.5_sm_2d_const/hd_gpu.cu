#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cublas.h>

#include "hd_block_size.h"
#include "spt2.h"

__global__ void horizontal_diffusion_gpu(const int ids, const int ide,
		const int jds, const int jde, const int kds, const int kde,
		const int ims, const int ime, const int jms, const int jme,
		const int kms, const int kme, const int its, const int ite,
		const int jts, const int jte, const int kts, const int kte,
		const int cf_specified, const int cf_nested, const int cf_open_xs,
		const int cf_open_xe, const int cf_open_ys, const int cf_open_ye,
		const int cf_periodic_x, const int cf_polar, const char name,
		const float * __restrict__ field, float * __restrict__ tendency, const float * __restrict__ mu,
		const float * __restrict__ msfux, const float * __restrict__ msfuy, const float * __restrict__ msfvx,
		const float * __restrict__ msfvx_inv, const float * __restrict__ msfvy, const float * __restrict__ msftx,
		const float * __restrict__ msfty, const float khdif, const float * __restrict__ xkmhd,
		const float rdx, const float rdy) {
	// Local variables
	//int i, j, k, ktf;
	int k;
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

	const int ktf = min(kte, kde - 1);

	// Thread coordinates
	const int i = bi * bx + ti;
	const int j = bj * by + tj;

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

		// Debug info:

		// Check bounds before calculations!
		if (!(i >= i_start + 4 && i <= i_end + 4 && j >= j_start + 4
				&& j <= j_end + 4)) {
			return;
		}

		// Redundant copy to shared memory
		// The copy pattern:
		//  *   *
		//    t
		//  *   *
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

		for (k = kts - 1; k <= ktf - 1; k++) {
			// The interior is grad: (m_x*d/dx), the exterior is div: (m_x*m_y*d/dx(/m_y))
			// setting up different averagings of m^2 partial d/dX and m^2 partial d/dY

			mkrdxm = (msftx_s[S2(ti-1,tj)] / msfty_s[S2(ti-1,tj)])
					* mu_s[S2(ti-1,tj)] * xkmhd[P3(ti-1,k,tj)] * rdx;
			mkrdxp = (msftx_s[S2(ti,tj)] / msfty_s[S2(ti,tj)]) * mu_s[S2(ti,tj)]
					* xkmhd[P3(ti,k,tj)] * rdx;
			mrdx = msfux_s[S2(ti,tj)] * msfuy_s[S2(ti,tj)] * rdx;

			mkrdym = ((msfuy_s[S2(ti,tj)] + msfuy_s[S2(ti,tj-1)])
					/ (msfux_s[S2(ti,tj)] + msfux_s[S2(ti,tj-1)])) * 0.25
					* (mu_s[S2(ti,tj)] + mu_s[S2(ti,tj-1)] + mu_s[S2(ti-1,tj-1)]
							+ mu_s[S2(ti-1,tj)]) * 0.25
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj-1)]
							+ xkmhd[P3(ti-1,k,tj-1)] + xkmhd[P3(ti-1,k,tj)])
					* rdy;
			mkrdyp = ((msfuy_s[S2(ti,tj)] + msfuy_s[S2(ti,tj+1)])
					/ (msfux_s[S2(ti,tj)] + msfux_s[S2(ti,tj+1)])) * 0.25
					* (mu_s[S2(ti,tj)] + mu_s[S2(ti,tj+1)] + mu_s[S2(ti-1,tj+1)]
							+ mu_s[S2(ti-1,tj)]) * 0.25
					* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj+1)]
							+ xkmhd[P3(ti-1,k,tj+1)] + xkmhd[P3(ti-1,k,tj)])
					* rdy;

			// need to do four-corners (t) for diffusion coefficient as there are
			// no values at u,v points
			// msfuy - has to be y as part of d/dY
			//         has to be u as we're at a u point
			mrdy = msfux_s[S2(ti,tj)] * msfuy_s[S2(ti,tj)] * rdy;

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
		if (!(i >= i_start - 1 && i <= i_end - 1 && j >= j_start - 1
				&& j <= j_end - 1)) {
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
		if (!(i >= i_start - 1 && i <= i_end - 1 && j >= j_start - 1
				&& j <= j_end - 1)) {
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
