#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "spt2.h"

void horizontal_diffusion_gpu(int ids, int ide, int jds, int jde,
		int kds, int kde, int ims, int ime, int jms, int jme, int kms, int kme,
		int its, int ite, int jts, int jte, int kts, int kte, int cf_specified,
		int cf_nested, int cf_open_xs, int cf_open_xe, int cf_open_ys,
		int cf_open_ye, int cf_periodic_x, int cf_polar, char name,
		float *field, float *tendency, float *mu, float *msfux, float *msfuy,
		float *msfvx, float *msfvx_inv, float *msfvy, float *msftx, float *msfty,
		float khdif, float *xkmhd, float rdx, float rdy){

	// Local variables
	int i, j, k, ktf;
	int i_start, i_end, j_start, j_end;
	float mrdx, mkrdxm, mkrdxp, mrdy, mkrdym, mkrdyp;
	int specified = 0;

	if(cf_specified || cf_nested) {
		specified = 1;
	}

	ktf=MIN(kte,kde-1);

	if(name == 'u') {
		i_start = its;
		i_end = ite;
		j_start = jts;
		j_end = MIN(jte,jde-1);

		if( cf_open_xs || specified ) i_start = MAX(ids+1,its);
		if( cf_open_xe || specified ) i_end = MIN(ide-1,ite);
		if( cf_open_ys || specified ) j_start = MAX(jds+1,jts);
		if( cf_open_ye || specified ) j_end = MIN(jde-2,jte);
		if( cf_periodic_x ) i_start = its;
		if( cf_periodic_x ) i_end = ite;

		i_start = i_start + 10;
		i_end = i_end - 10;
		j_start = j_start + 10;
		j_end = j_end - 10;
		kts = kts + 10;
		ktf = ktf - 10;
		printf("Indices de trabajo originales: (%d, %d); (%d, %d); (%d, %d);\n", i_start, i_end, j_start, j_end, kts, ktf);

		for( j=indexJ(j_start); j<indexJ(j_end); j++) {
		for( k=indexK(kts); k<indexK(ktf); k++) {
		for( i=indexI(i_start); i<indexI(i_end); i++) {

			// The interior is grad: (m_x*d/dx), the exterior is div: (m_x*m_y*d/dx(/m_y))
			// setting up different averagings of m^2 partial d/dX and m^2 partial d/dY

			mkrdxm=( msftx[P2(i-1,j)]/msfty[P2(i-1,j)] ) * mu[P2(i-1,j)] * xkmhd[P3(i-1,k,j)] * rdx;
			mkrdxp=( msftx[P2(i,j)]/msfty[P2(i,j)])*mu[P2(i,j)]*xkmhd[P3(i,k,j)]*rdx;
			mrdx=msfux[P2(i,j)]*msfuy[P2(i,j)]*rdx;

			/*mkrdym=( (msfuy[P2(i,j)]+msfuy[P2(i,j-1)])/(msfux[P2(i,j)]+msfux[P2(i,j-1)]) )*
					0.25*(mu[P2(i,j)]+mu[P2(i,j-1)]+mu[P2(i-1,j-1)]+mu[P2(i-1,j)])*
					0.25*(xkmhd[P3(i,k,j)]+xkmhd[P3(i,k,j-1)]+xkmhd[P3(i-1,k,j-1)]+xkmhd[P3(i-1,k,j)])*rdy;*/
			mkrdyp=( (msfuy[P2(i,j)]+msfuy[P2(i,j+1)])/(msfux[P2(i,j)]+msfux[P2(i,j+1)]) )*
					0.25*(mu[P2(i,j)]+mu[P2(i,j+1)]+mu[P2(i-1,j+1)]+mu[P2(i-1,j)])*
					0.25*(xkmhd[P3(i,k,j)]+xkmhd[P3(i,k,j+1)]+xkmhd[P3(i-1,k,j+1)]+xkmhd[P3(i-1,k,j)])*rdy;

			// need to do four-corners (t) for diffusion coefficient as there are
			// no values at u,v points
			// msfuy - has to be y as part of d/dY
			//         has to be u as we're at a u point
			mrdy=msfux[P2(i,j)]*msfuy[P2(i,j)]*rdy;
/*
			// correctly averaged version of rho~ * m^2 *
			//    [partial d/dX(partial du^/dX) + partial d/dY(partial du^/dY)]
			tendency[P3(i,k,j)]=tendency[P3(i,k,j)]+(
								mrdx*(mkrdxp*(field[P3(i+1,k,j)]-field[P3(i,k,j)])
										-mkrdxm*(field[P3(i,k,j)]-field[P3(i-1,k,j)]))
										+mrdy*(mkrdyp*(field[P3(i,k,j+1)]-field[P3(i,k,j)])
												-mkrdym*(field[P3(i,k,j)]-field[P3(i,k,j-1)])));
*/

			//printf("%f", mkrdym);

			tendency[P3(i,k,j)]=tendency[P3(i,k,j)]+10 + (
								mrdx*(mkrdxp*(field[P3(i+1,k,j)]-field[P3(i,k,j)])
								-mkrdxm*(field[P3(i,k,j)]-field[P3(i-1,k,j)]))
								+mrdy*(mkrdyp*(field[P3(i,k,j+1)]-field[P3(i,k,j)])
								));




  		}}}
	}else
		if(name == 'v') {

		i_start = its;
		i_end = MIN(ite,ide-1);
		j_start = jts;
		j_end = jte;

		if( cf_open_xs || specified ) i_start = MAX(ids+1,its);
		if( cf_open_xe || specified ) i_end = MIN(ide-2,ite);
		if( cf_open_ys || specified ) j_start = MAX(jds+1,jts);
		if( cf_open_ye || specified ) j_end = MIN(jde-1,jte);
		if( cf_periodic_x ) i_start = its;
		if( cf_periodic_x ) i_end = MIN(ite,ide-1);
		if( cf_polar ) j_start = MAX(jds+1,jts);
		if( cf_polar ) j_end = MIN(jde-1,jte);


		for( k=kts-1; k<=ktf-1; k++ ) {

			mkrdxm=( (msfvx[P2(i,j)]+msfvx[P2(i-1,j)])/(msfvy[P2(i,j)]+msfvy[P2(i-1,j)]) )*
					0.25*(mu[P2(i,j)]+mu[P2(i,j-1)]+mu[P2(i-1,j-1)]+mu[P2(i-1,j)])*
					0.25*(xkmhd[P3(i,k,j)]+xkmhd[P3(i,k,j-1)]+xkmhd[P3(i-1,k,j-1)]+xkmhd[P3(i-1,k,j)])*rdx;
			mkrdxp=( (msfvx[P2(i,j)]+msfvx[P2(i+1,j)])/(msfvy[P2(i,j)]+msfvy[P2(i+1,j)]) )*
					0.25*(mu[P2(i,j)]+mu[P2(i,j-1)]+mu[P2(i+1,j-1)]+mu[P2(i+1,j)])*
					0.25*(xkmhd[P3(i,k,j)]+xkmhd[P3(i,k,j-1)]+xkmhd[P3(i+1,k,j-1)]+xkmhd[P3(i+1,k,j)])*rdx;
			mrdx=msfvx[P2(i,j)]*msfvy[P2(i,j)]*rdx;
			mkrdym=(msfty[P2(i,j-1)]/msftx[P2(i,j-1)])*xkmhd[P3(i,k,j-1)]*rdy;
			mkrdyp=(msfty[P2(i,j)]/msftx[P2(i,j)])*xkmhd[P3(i,k,j)]*rdy;
			mrdy=msfvx[P2(i,j)]*msfvy[P2(i,j)]*rdy;

			tendency[P3(i,k,j)]=tendency[P3(i,k,j)]+(
					mrdx*(mkrdxp*(field[P3(i+1,k,j)]-field[P3(i,k,j)])
							-mkrdxm*(field[P3(i,k,j)]-field[P3(i-1,k,j)]))
							+mrdy*(mkrdyp*(field[P3(i,k,j+1)]-field[P3(i,k,j)])
									-mkrdym*(field[P3(i,k,j)]-field[P3(i,k,j-1)])));
		}
	}else if(name == 'w') {

		i_start = its;
		i_end = MIN(ite,ide-1);
		j_start = jts;
		j_end = MIN(jte,jde-1);

		if( cf_open_xs || specified ) i_start = MAX(ids+1,its);
		if( cf_open_xe || specified ) i_end = MIN(ide-2,ite);
		if( cf_open_ys || specified ) j_start = MAX(jds+1,jts);
		if( cf_open_ye || specified ) j_end = MIN(jde-2,jte);
		if( cf_periodic_x ) i_start = its;
		if( cf_periodic_x ) i_end = MIN(ite,ide-1);


		for( k=kts-1; k<=ktf-1; k++ ) {

			mkrdxm=(msfux[P2(i,j)]/msfuy[P2(i,j)])*
					0.25*(mu[P2(i,j)]+mu[P2(i-1,j)]+mu[P2(i,j)]+mu[P2(i-1,j)])*
					0.25*(xkmhd[P3(i,k,j)]+xkmhd[P3(i-1,k,j)]+xkmhd[P3(i,k-1,j)]+xkmhd[P3(i-1,k-1,j)])*rdx;
			mkrdxp=(msfux[P2(i+1,j)]/msfuy[P2(i+1,j)])*
					0.25*(mu[P2(i+1,j)]+mu[P2(i,j)]+mu[P2(i+1,j)]+mu[P2(i,j)])*
					0.25*(xkmhd[P3(i+1,k,j)]+xkmhd[P3(i,k,j)]+xkmhd[P3(i+1,k-1,j)]+xkmhd[P3(i,k-1,j)])*rdx;
			mrdx=msftx[P2(i,j)]*msfty[P2(i,j)]*rdx;
			//         mkrdym=(msfvy[P2(i,j)]/msfvx[P2(i,j)])*
			mkrdym=(msfvy[P2(i,j)]*msfvx_inv[P2(i,j)])*
					0.25*(mu[P2(i,j)]+mu[P2(i,j-1)]+mu[P2(i,j)]+mu[P2(i,j-1)])*
					0.25*(xkmhd[P3(i,k,j)]+xkmhd[P3(i,k,j-1)]+xkmhd[P3(i,k-1,j)]+xkmhd[P3(i,k-1,j-1)])*rdy;
			//         mkrdyp=(msfvy[P2(i,j+1)]/msfvx[P2(i,j+1)])*
			mkrdyp=(msfvy[P2(i,j+1)]*msfvx_inv[P2(i,j+1)])*
					0.25*(mu[P2(i,j+1)]+mu[P2(i,j)]+mu[P2(i,j+1)]+mu[P2(i,j)])*
					0.25*(xkmhd[P3(i,k,j+1)]+xkmhd[P3(i,k,j)]+xkmhd[P3(i,k-1,j+1)]+xkmhd[P3(i,k-1,j)])*rdy;
			mrdy=msftx[P2(i,j)]*msfty[P2(i,j)]*rdy;

			tendency[P3(i,k,j)]=tendency[P3(i,k,j)]+(
					mrdx*(mkrdxp*(field[P3(i+1,k,j)]-field[P3(i,k,j)])
							-mkrdxm*(field[P3(i,k,j)]-field[P3(i-1,k,j)]))
							+mrdy*(mkrdyp*(field[P3(i,k,j+1)]-field[P3(i,k,j)])
									-mkrdym*(field[P3(i,k,j)]-field[P3(i,k,j-1)])));
		}
	}else {

		i_start = its;
		i_end = MIN(ite,ide-1);
		j_start = jts;
		j_end = MIN(jte,jde-1);

		if( cf_open_xs || specified ) i_start = MAX(ids+1,its);
		if( cf_open_xe || specified ) i_end = MIN(ide-2,ite);
		if( cf_open_ys || specified ) j_start = MAX(jds+1,jts);
		if( cf_open_ye || specified ) j_end = MIN(jde-2,jte);
		if( cf_periodic_x ) i_start = its;
		if( cf_periodic_x ) i_end = MIN(ite,ide-1);

		for( k=kts; k<=ktf; k++ ) {

			mkrdxm=(msfux[P2(i,j)]/msfuy[P2(i,j)])*0.5*(xkmhd[P3(i,k,j)]+xkmhd[P3(i-1,k,j)])*0.5*(mu[P2(i,j)]+mu[P2(i-1,j)])*rdx;
			mkrdxp=(msfux[P2(i+1,j)]/msfuy[P2(i+1,j)])*0.5*(xkmhd[P3(i+1,k,j)]+xkmhd[P3(i,k,j)])*0.5*(mu[P2(i+1,j)]+mu[P2(i,j)])*rdx;
			mrdx=msftx[P2(i,j)]*msfty[P2(i,j)]*rdx;
			//         mkrdym=(msfvy[P2(i,j)]/msfvx[P2(i,j)])*0.5*(xkmhd[P3(i,k,j)]+xkmhd[P3(i,k,j-1)])*0.5*(mu[P2(i,j)]+mu[P2(i,j-1)])*rdy
			mkrdym=(msfvy[P2(i,j)]*msfvx_inv[P2(i,j)])*0.5*(xkmhd[P3(i,k,j)]+xkmhd[P3(i,k,j-1)])*0.5*(mu[P2(i,j)]+mu[P2(i,j-1)])*rdy;
			//         mkrdyp=(msfvy[P2(i,j+1)]/msfvx[P2(i,j+1)])*0.5*(xkmhd[P3(i,k,j+1)]+xkmhd[P3(i,k,j)])*0.5*(mu[P2(i,j+1)]+mu[P2(i,j)])*rdy
			mkrdyp=(msfvy[P2(i,j+1)]*msfvx_inv[P2(i,j+1)])*0.5*(xkmhd[P3(i,k,j+1)]+xkmhd[P3(i,k,j)])*0.5*(mu[P2(i,j+1)]+mu[P2(i,j)])*rdy;
			mrdy=msftx[P2(i,j)]*msfty[P2(i,j)]*rdy;

			tendency[P3(i,k,j)]=tendency[P3(i,k,j)]+(
					mrdx*(mkrdxp*(field[P3(i+1,k,j)]-field[P3(i,k,j)])
							-mkrdxm*(field[P3(i,k,j)]-field[P3(i-1,k,j)]))
							+mrdy*(mkrdyp*(field[P3(i,k,j+1)]-field[P3(i,k,j)])
									-mkrdym*(field[P3(i,k,j)]-field[P3(i,k,j-1)])));
		}
	}


}
