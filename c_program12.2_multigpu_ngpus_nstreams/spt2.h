#define bi   blockIdx.x
#define bj   blockIdx.y
#define bx   blockDim.x
#define by   blockDim.y
#define ti   threadIdx.x
#define tj   threadIdx.y

// Indices for accessing device memory - taking into account sub-domain decomposition
#define Q2(i,j)    (bi*bx+i + (bj*by+j)*ix)
#define Q3(i,k,j)  ((bi*bx+i) + k*ix + (bj*by+j)*ix*kx)

#define S2(i,j)  ((i+1)+((j+1)*(XXX+2)))
#define S3(i,k,j)  ((i+1)+((j+1)*(XXX+2)))

// Min & max
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif


