#include <stdio.h>
#include "spt2.h"

extern void horizontal_diffusion_gpu(int ids, int ide, int jds, int jde,
		int kds, int kde, int ims, int ime, int jms, int jme, int kms, int kme,
		int its, int ite, int jts, int jte, int kts, int kte, int cf_specified,
		int cf_nested, int cf_open_xs, int cf_open_xe, int cf_open_ys,
		int cf_open_ye, int cf_periodic_x, int cf_polar, char name,
		float *field, float *tendency, float *mu, float *msfux, float *msfuy,
		float *msfvx, float *msfvx_inv, float *msfvy, float *msftx, float *msfty,
		float khdif, float *xkmhd, float rdx, float rdy);


extern int gethostname(char * name, size_t len);

/**
 * Print variable to console
 */
void printVariable(const char name[], float *var, int ims, int ime, int kms, int kme, int jms, int jme){
	printf("%s:\n", name);
	
	int i,j,k;
	
	for(k=indexK(kms); k<=indexK(kme); k++){
		for(j = indexJ(jms); j<=indexJ(jme); j++){
			for(i = indexI(ims); i<=indexI(ime); i++){
				printf("%7.2f\t", var[i + k*IX + j*IX*KX]);
			}
			printf("\n");
		}
		printf("*\n");
	}
}


//[435][35][310]
int horizontal_diffusion_host_(int *ids, int *ide, int *jds, int *jde, int *kds, int *kde,
		int *ims, int *ime, int *jms, int *jme, int *kms, int *kme,
		int *its, int *ite, int *jts, int *jte, int *kts, int *kte,
		int *cf_specified, int *cf_nested, int *cf_open_xs, int *cf_open_xe, int *cf_open_ys,
		int *cf_open_ye, int *cf_periodic_x, int *cf_polar, char *name,
		float *field, float *tendency, float *mu, float *msfux, float *msfuy,
		float *msfvx, float *msfvx_inv,
		float *msfvy, float *msftx, float *msfty,
		float *khdif, float *xkmhd, float *rdx, float *rdy) {

	// Dimensions
	int d3 = (*ime - *ims + 1) * (*jme - *jms + 1) * (*kme - *kms + 1);
	int d2 = (*ime - *ims + 1) * (*jme - *jms + 1);

	printf("Dimensions:\n");
	printf("ids,ide,jds,jde,kds,kde: %d,%d,%d,%d,%d,%d\n", *ids, *ide, *jds, *jde, *kds, *kde);
	printf("ims,ime,jms,jme,kms,kme: %d,%d,%d,%d,%d,%d\n", *ims, *ime, *jms, *jme, *kms, *kme);
	printf("its,ite,jts,jte,kts,kte: %d,%d,%d,%d,%d,%d\n", *its, *ite, *jts, *jte, *kts, *kte);


	horizontal_diffusion_gpu(*ids, *ide, *jds, *jde,
			*kds, *kde, *ims, *ime, *jms, *jme, *kms, *kme,
			*its, *ite, *jts, *jte, *kts, *kte, *cf_specified,
			*cf_nested, *cf_open_xs, *cf_open_xe, *cf_open_ys,
			*cf_open_ye, *cf_periodic_x, *cf_polar, *name,
			field, tendency, mu, msfux, msfuy,
			msfvx, msfvx_inv, msfvy, msftx, msfty,
			*khdif, xkmhd, *rdx, *rdy);


	return 0;
}

