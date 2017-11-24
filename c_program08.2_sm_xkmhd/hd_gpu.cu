#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cublas.h>

#include "hd_block_size.h"
#include "spt2.h"


__constant__ float rdx_[1];
__constant__ float rdy_[1];

__global__ void horizontal_diffusion_gpu(int ids, int ide, int jds, int jde,
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
	
	// Esto hace que aumente considerablemente el tiempo (+40%)
	// en lugar de disminuirlo

	__shared__ float xkmhd_s[((XXX+2)*(YYY+2)*(MKX))];

	if(cf_specified || cf_nested) {
		specified = 1;
	}

	ktf=min(kte,kde-1);

	// Thread coordinates
	i = bi*bx+ti;
	j = bj*by+tj;

	/*for( k=kts-1; k<=kte-1; k++) {
		if( !( i>=i_start-1 && i<=i_end-1 && j>=j_start-1 && j<=j_end-1 ) ){
			return;
		}
		tendency[P3(ti,k,tj)]=tendency[P3(ti,k,tj)]+2;
	} return;*/

	if(name == 'u') {
		i_start = its;
		i_end = ite;
		j_start = jts;
		j_end = min(jte,jde-1);

		if( cf_open_xs || specified ) i_start = max(ids+1,its);
		if( cf_open_xe || specified ) i_end = min(ide-1,ite);
		if( cf_open_ys || specified ) j_start = max(jds+1,jts);
		if( cf_open_ye || specified ) j_end = min(jde-2,jte);
		if( cf_periodic_x ) i_start = its;
		if( cf_periodic_x ) i_end = ite;

		// Debug info:
		/*if(ti==0){
			tendency[0]=i_start;tendency[1]=i_end;tendency[2]=j_start;tendency[3]=j_end;
			tendency[4]=kts;tendency[5]=ktf;
		}*/

		// Check bounds before calculations!
		//if( !( i>=i_start-1 && i<=i_end-1 && j>=j_start-1 && j<=j_end-1 ) ){
		if( !( i>=i_start+4 && i<=i_end+4 && j>=j_start+4 && j<=j_end+4 ) ){
			// Debug
//			if(i<=ime-ims+1 && j<=jme-jms+1){
//				tendency[P3(ti,0,tj)] = -99.88;
//			}
			return;
		}
		
				// Redundant copy to shared memory
				for( k=kts-1; k<=ktf-1; k++) {
					xkmhd_s[S3(ti-1,k,tj-1)] = xkmhd[P3(ti-1,k,tj-1)];
					xkmhd_s[S3(ti+1,k,tj-1)] = xkmhd[P3(ti+1,k,tj-1)];
					xkmhd_s[S3(ti-1,k,tj+1)] = xkmhd[P3(ti-1,k,tj+1)];
					xkmhd_s[S3(ti+1,k,tj+1)] = xkmhd[P3(ti+1,k,tj+1)];
				}

				__syncthreads();


		for( k=kts-1; k<=ktf-1; k++) {
			// The interior is grad: (m_x*d/dx), the exterior is div: (m_x*m_y*d/dx(/m_y))
			// setting up different averagings of m^2 partial d/dX and m^2 partial d/dY

			mkrdxm=( msftx[P2(ti-1,tj)]/msfty[P2(ti-1,tj)] ) * mu[P2(ti-1,tj)] * xkmhd_s[S3(ti-1,k,tj)] * rdx;
			mkrdxp=( msftx[P2(ti,tj)]/msfty[P2(ti,tj)])*mu[P2(ti,tj)]*xkmhd_s[S3(ti,k,tj)]*rdx;
			mrdx=msfux[P2(ti,tj)]*msfuy[P2(ti,tj)]*rdx;

			mkrdym=( (msfuy[P2(ti,tj)]+msfuy[P2(ti,tj-1)])/(msfux[P2(ti,tj)]+msfux[P2(ti,tj-1)]) )*
					0.25*(mu[P2(ti,tj)]+mu[P2(ti,tj-1)]+mu[P2(ti-1,tj-1)]+mu[P2(ti-1,tj)])*
					0.25*(xkmhd_s[S3(ti,k,tj)]+xkmhd_s[S3(ti,k,tj-1)]+xkmhd_s[S3(ti-1,k,tj-1)]+xkmhd_s[S3(ti-1,k,tj)])*rdy;
			mkrdyp=( (msfuy[P2(ti,tj)]+msfuy[P2(ti,tj+1)])/(msfux[P2(ti,tj)]+msfux[P2(ti,tj+1)]) )*
					0.25*(mu[P2(ti,tj)]+mu[P2(ti,tj+1)]+mu[P2(ti-1,tj+1)]+mu[P2(ti-1,tj)])*
					0.25*(xkmhd_s[S3(ti,k,tj)]+xkmhd_s[S3(ti,k,tj+1)]+xkmhd_s[S3(ti-1,k,tj+1)]+xkmhd_s[S3(ti-1,k,tj)])*rdy;

			// need to do four-corners (t) for diffusion coefficient as there are
			// no values at u,v points
			// msfuy - has to be y as part of d/dY
			//         has to be u as we're at a u point
			mrdy=msfux[P2(ti,tj)]*msfuy[P2(ti,tj)]*rdy;

			// correctly averaged version of rho~ * m^2 *
			//    [partial d/dX(partial du^/dX) + partial d/dY(partial du^/dY)]
			/*tendency[P3(ti,k,tj)]=tendency[P3(ti,k,tj)]+(
					mrdx*(mkrdxp*(field[P3(ti+1,k,tj)]-field[P3(ti,k,tj)])
							-mkrdxm*(field[P3(ti,k,tj)]-field[P3(ti-1,k,tj)]))
							+mrdy*(mkrdyp*(field[P3(ti,k,tj+1)]-field[P3(ti,k,tj)])
									-mkrdym*(field[P3(ti,k,tj)]-field[P3(ti,k,tj-1)])));*/
			tendency[P3(ti,k,tj)]=tendency[P3(ti,k,tj)]+(
								mrdx*(mkrdxp*(field[P3(ti+1,k,tj)]-field[P3(ti,k,tj)])
										-mkrdxm*(field[P3(ti,k,tj)]-field[P3(ti-1,k,tj)]))
										+mrdy*(mkrdyp*(field[P3(ti,k,tj+1)]-field[P3(ti,k,tj)])
												-mkrdym*(field[P3(ti,k,tj)]-field[P3(ti,k,tj-1)])));
		}
	}else
		if(name == 'v') {

		i_start = its;
		i_end = min(ite,ide-1);
		j_start = jts;
		j_end = jte;

		if( cf_open_xs || specified ) i_start = max(ids+1,its);
		if( cf_open_xe || specified ) i_end = min(ide-2,ite);
		if( cf_open_ys || specified ) j_start = max(jds+1,jts);
		if( cf_open_ye || specified ) j_end = min(jde-1,jte);
		if( cf_periodic_x ) i_start = its;
		if( cf_periodic_x ) i_end = min(ite,ide-1);
		if( cf_polar ) j_start = max(jds+1,jts);
		if( cf_polar ) j_end = min(jde-1,jte);

		// Check bounds before calculations!
		if( !( i>=i_start-1 && i<=i_end-1 && j>=j_start-1 && j<=j_end-1 ) ){
			return;
		}

		for( k=kts-1; k<=ktf-1; k++ ) {

			mkrdxm=( (msfvx[P2(ti,tj)]+msfvx[P2(ti-1,tj)])/(msfvy[P2(ti,tj)]+msfvy[P2(ti-1,tj)]) )*
					0.25*(mu[P2(ti,tj)]+mu[P2(ti,tj-1)]+mu[P2(ti-1,tj-1)]+mu[P2(ti-1,tj)])*
					0.25*(xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti,k,tj-1)]+xkmhd[P3(ti-1,k,tj-1)]+xkmhd[P3(ti-1,k,tj)])*rdx;
			mkrdxp=( (msfvx[P2(ti,tj)]+msfvx[P2(ti+1,tj)])/(msfvy[P2(ti,tj)]+msfvy[P2(ti+1,tj)]) )*
					0.25*(mu[P2(ti,tj)]+mu[P2(ti,tj-1)]+mu[P2(ti+1,tj-1)]+mu[P2(ti+1,tj)])*
					0.25*(xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti,k,tj-1)]+xkmhd[P3(ti+1,k,tj-1)]+xkmhd[P3(ti+1,k,tj)])*rdx;
			mrdx=msfvx[P2(ti,tj)]*msfvy[P2(ti,tj)]*rdx;
			mkrdym=(msfty[P2(ti,tj-1)]/msftx[P2(ti,tj-1)])*xkmhd[P3(ti,k,tj-1)]*rdy;
			mkrdyp=(msfty[P2(ti,tj)]/msftx[P2(ti,tj)])*xkmhd[P3(ti,k,tj)]*rdy;
			mrdy=msfvx[P2(ti,tj)]*msfvy[P2(ti,tj)]*rdy;

			tendency[P3(ti,k,tj)]=tendency[P3(ti,k,tj)]+(
					mrdx*(mkrdxp*(field[P3(ti+1,k,tj)]-field[P3(ti,k,tj)])
							-mkrdxm*(field[P3(ti,k,tj)]-field[P3(ti-1,k,tj)]))
							+mrdy*(mkrdyp*(field[P3(ti,k,tj+1)]-field[P3(ti,k,tj)])
									-mkrdym*(field[P3(ti,k,tj)]-field[P3(ti,k,tj-1)])));
		}
	}else if(name == 'w') {

		i_start = its;
		i_end = min(ite,ide-1);
		j_start = jts;
		j_end = min(jte,jde-1);

		if( cf_open_xs || specified ) i_start = max(ids+1,its);
		if( cf_open_xe || specified ) i_end = min(ide-2,ite);
		if( cf_open_ys || specified ) j_start = max(jds+1,jts);
		if( cf_open_ye || specified ) j_end = min(jde-2,jte);
		if( cf_periodic_x ) i_start = its;
		if( cf_periodic_x ) i_end = min(ite,ide-1);

		// Check bounds before calculations!
		if( !( i>=i_start-1 && i<=i_end-1 && j>=j_start-1 && j<=j_end-1 ) ){
			return;
		}

		for( k=kts-1; k<=ktf-1; k++ ) {

			mkrdxm=(msfux[P2(ti,tj)]/msfuy[P2(ti,tj)])*
					0.25*(mu[P2(ti,tj)]+mu[P2(ti-1,tj)]+mu[P2(ti,tj)]+mu[P2(ti-1,tj)])*
					0.25*(xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti-1,k,tj)]+xkmhd[P3(ti,k-1,tj)]+xkmhd[P3(ti-1,k-1,tj)])*rdx;
			mkrdxp=(msfux[P2(ti+1,tj)]/msfuy[P2(ti+1,tj)])*
					0.25*(mu[P2(ti+1,tj)]+mu[P2(ti,tj)]+mu[P2(ti+1,tj)]+mu[P2(ti,tj)])*
					0.25*(xkmhd[P3(ti+1,k,tj)]+xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti+1,k-1,tj)]+xkmhd[P3(ti,k-1,tj)])*rdx;
			mrdx=msftx[P2(ti,tj)]*msfty[P2(ti,tj)]*rdx;
			//         mkrdym=(msfvy[P2(ti,tj)]/msfvx[P2(ti,tj)])*
			mkrdym=(msfvy[P2(ti,tj)]*msfvx_inv[P2(ti,tj)])*
					0.25*(mu[P2(ti,tj)]+mu[P2(ti,tj-1)]+mu[P2(ti,tj)]+mu[P2(ti,tj-1)])*
					0.25*(xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti,k,tj-1)]+xkmhd[P3(ti,k-1,tj)]+xkmhd[P3(ti,k-1,tj-1)])*rdy;
			//         mkrdyp=(msfvy[P2(ti,tj+1)]/msfvx[P2(ti,tj+1)])*
			mkrdyp=(msfvy[P2(ti,tj+1)]*msfvx_inv[P2(ti,tj+1)])*
					0.25*(mu[P2(ti,tj+1)]+mu[P2(ti,tj)]+mu[P2(ti,tj+1)]+mu[P2(ti,tj)])*
					0.25*(xkmhd[P3(ti,k,tj+1)]+xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti,k-1,tj+1)]+xkmhd[P3(ti,k-1,tj)])*rdy;
			mrdy=msftx[P2(ti,tj)]*msfty[P2(ti,tj)]*rdy;

			tendency[P3(ti,k,tj)]=tendency[P3(ti,k,tj)]+(
					mrdx*(mkrdxp*(field[P3(ti+1,k,tj)]-field[P3(ti,k,tj)])
							-mkrdxm*(field[P3(ti,k,tj)]-field[P3(ti-1,k,tj)]))
							+mrdy*(mkrdyp*(field[P3(ti,k,tj+1)]-field[P3(ti,k,tj)])
									-mkrdym*(field[P3(ti,k,tj)]-field[P3(ti,k,tj-1)])));
		}
	}else {

		i_start = its;
		i_end = min(ite,ide-1);
		j_start = jts;
		j_end = min(jte,jde-1);

		if( cf_open_xs || specified ) i_start = max(ids+1,its);
		if( cf_open_xe || specified ) i_end = min(ide-2,ite);
		if( cf_open_ys || specified ) j_start = max(jds+1,jts);
		if( cf_open_ye || specified ) j_end = min(jde-2,jte);
		if( cf_periodic_x ) i_start = its;
		if( cf_periodic_x ) i_end = min(ite,ide-1);

		for( k=kts; k<=ktf; k++ ) {

			mkrdxm=(msfux[P2(ti,tj)]/msfuy[P2(ti,tj)])*0.5*(xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti-1,k,tj)])*0.5*(mu[P2(ti,tj)]+mu[P2(ti-1,tj)])*rdx;
			mkrdxp=(msfux[P2(ti+1,tj)]/msfuy[P2(ti+1,tj)])*0.5*(xkmhd[P3(ti+1,k,tj)]+xkmhd[P3(ti,k,tj)])*0.5*(mu[P2(ti+1,tj)]+mu[P2(ti,tj)])*rdx;
			mrdx=msftx[P2(ti,tj)]*msfty[P2(ti,tj)]*rdx;
			//         mkrdym=(msfvy[P2(ti,tj)]/msfvx[P2(ti,tj)])*0.5*(xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti,k,tj-1)])*0.5*(mu[P2(ti,tj)]+mu[P2(ti,tj-1)])*rdy
			mkrdym=(msfvy[P2(ti,tj)]*msfvx_inv[P2(ti,tj)])*0.5*(xkmhd[P3(ti,k,tj)]+xkmhd[P3(ti,k,tj-1)])*0.5*(mu[P2(ti,tj)]+mu[P2(ti,tj-1)])*rdy;
			//         mkrdyp=(msfvy[P2(ti,tj+1)]/msfvx[P2(ti,tj+1)])*0.5*(xkmhd[P3(ti,k,tj+1)]+xkmhd[P3(ti,k,tj)])*0.5*(mu[P2(ti,tj+1)]+mu[P2(ti,tj)])*rdy
			mkrdyp=(msfvy[P2(ti,tj+1)]*msfvx_inv[P2(ti,tj+1)])*0.5*(xkmhd[P3(ti,k,tj+1)]+xkmhd[P3(ti,k,tj)])*0.5*(mu[P2(ti,tj+1)]+mu[P2(ti,tj)])*rdy;
			mrdy=msftx[P2(ti,tj)]*msfty[P2(ti,tj)]*rdy;

			tendency[P3(ti,k,tj)]=tendency[P3(ti,k,tj)]+(
					mrdx*(mkrdxp*(field[P3(ti+1,k,tj)]-field[P3(ti,k,tj)])
							-mkrdxm*(field[P3(ti,k,tj)]-field[P3(ti-1,k,tj)]))
							+mrdy*(mkrdyp*(field[P3(ti,k,tj+1)]-field[P3(ti,k,tj)])
									-mkrdym*(field[P3(ti,k,tj)]-field[P3(ti,k,tj-1)])));
		}
	}

}
