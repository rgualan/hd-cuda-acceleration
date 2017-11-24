#define SM_SIZE  (0x1000-0xd4)
#define MAX_THREADS_PER_BLOCK  1024

#define bi   blockIdx.x
#define bj   blockIdx.y
#define bx   blockDim.x
#define by   blockDim.y
#define ti   threadIdx.x
#define tj   threadIdx.y

#define ix   (ime-ims+1)
#define jx   (jme-jms+1)
#define kx   (kme-kms+1)


#define P2(i,j)    (bi*bx+i + (bj*by+j)*ix )
#define P3(i,k,j)  ((bi*bx+i) + k*ix + (bj*by+j)*ix*kx)

#define S2(i,j)  ((i+1)+((j+1)*(XXX+2)))
#define S3(i,k,j)  ((i+1)+((j+1)*(XXX+2)))


// Min & max
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif




/**
 * Convert fortran index to c index
 */
#define I(fi) (fi+4)
#define J(fj) (fj+4)
#define K(fk) (fk-1)

