#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cublas.h>

#include "hd_block_size.h"
#include "spt2.h"

//__constant__ float rdx_[1];
//__constant__ float rdy_[1];

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
		const float rdx, const float rdy) {

	// Local variables
	//int i, j, k, ktf;

	int i_start, i_end, j_start, j_end;
	float mrdx, mkrdxm, mkrdxp, mrdy, mkrdym, mkrdyp;
	int specified = 0;

	// Esto hace que aumente considerablemente el tiempo (+40%)
	// en lugar de disminuirlo

	if (cf_specified || cf_nested) {
		specified = 1;
	}

	const int ktf = min(kte, kde - 1);

	// Thread coordinates
	const int i = bi * bx + ti;
	const int j = bj * by + tj;
	const int k = blockIdx.z * blockDim.z + threadIdx.z;

//  if (name == 'u') {
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
	if (!(i >= i_start + 4 && i <= i_end + 4 && j >= j_start + 4
			&& j <= j_end + 4 && k <= ktf - 1)) {
		return;
	}

	// The interior is grad: (m_x*d/dx), the exterior is div: (m_x*m_y*d/dx(/m_y))
	// setting up different averagings of m^2 partial d/dX and m^2 partial d/dY

	mkrdxm = (msftx[P2(ti-1,tj)] / msfty[P2(ti-1,tj)]) * mu[P2(ti-1,tj)]
			* xkmhd[P3(ti-1,k,tj)] * rdx;
	mkrdxp = (msftx[P2(ti,tj)] / msfty[P2(ti,tj)]) * mu[P2(ti,tj)]
			* xkmhd[P3(ti,k,tj)] * rdx;
	mrdx = msfux[P2(ti,tj)] * msfuy[P2(ti,tj)] * rdx;

	mkrdym = ((msfuy[P2(ti,tj)] + msfuy[P2(ti,tj-1)])
			/ (msfux[P2(ti,tj)] + msfux[P2(ti,tj-1)])) * 0.25
			* (mu[P2(ti,tj)] + mu[P2(ti,tj-1)] + mu[P2(ti-1,tj-1)]
					+ mu[P2(ti-1,tj)]) * 0.25
			* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj-1)]
					+ xkmhd[P3(ti-1,k,tj-1)] + xkmhd[P3(ti-1,k,tj)]) * rdy;
	mkrdyp = ((msfuy[P2(ti,tj)] + msfuy[P2(ti,tj+1)])
			/ (msfux[P2(ti,tj)] + msfux[P2(ti,tj+1)])) * 0.25
			* (mu[P2(ti,tj)] + mu[P2(ti,tj+1)] + mu[P2(ti-1,tj+1)]
					+ mu[P2(ti-1,tj)]) * 0.25
			* (xkmhd[P3(ti,k,tj)] + xkmhd[P3(ti,k,tj+1)]
					+ xkmhd[P3(ti-1,k,tj+1)] + xkmhd[P3(ti-1,k,tj)]) * rdy;

	// need to do four-corners (t) for diffusion coefficient as there are
	// no values at u,v points
	// msfuy - has to be y as part of d/dY
	//         has to be u as we're at a u point
	mrdy = msfux[P2(ti,tj)] * msfuy[P2(ti,tj)] * rdy;

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
									* (field[P3(ti+1,k,tj)] - field[P3(ti,k,tj)])
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
	/*	} else if (name == 'v') {

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
	 */
}
