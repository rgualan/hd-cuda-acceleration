//#define ix   (*ime-*ims+1)
//#define jx   (*jme-*jms+1)
//#define kx   (*kme-*kms+1)

//#define MAX(x,y) max(x,y)
//#define MIN(x,y) min(x,y)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define P2(i,j)    ((i) + (j)*(ix) )
#define P3(i,k,j)  ((i) + (k)*(ix) + (j)*(ix)*(kx))
//#define P3(i,k,j)  ((j) + (k)*jx + (i)*jx*kx)
//#define P3(i,k,j)  ((j) + (k)*jx + (i)*jx*kx)

/**
 * Convert fortran index to c index
 */
#define I(fi) fi+4
#define J(fj) fj+4
#define K(fk) fk-1

