// macros and whatnot for translator

#if 0
Types

1. Array stored in device memory


  1.a   fully dimensional
        Example 3D
            name:             qc
            how allocated:    argument
            dimensionality:   ims:ime,kms:kme,jms:jme
            index as:         P3(ti,k,tj)

        Example 2D
            name:             qc
            how allocated:    argument
            dimensionality:   ims:ime,jms:jme
            index as:         P2(ti,k,tj)


  1.b   1 dimensional (vertical only), local storage
        Example
            name:             w3
            how allocated:    local
            dimensionality:   constant (MKX)
            index as:         w3[k]

2. Array stored in shared memory

  2.a   fully dimensional

        3D
        Example
            name:             qc_s
            how allocated:    as offsets into sm[SM_SIZE]
            dimensionality:   bx*by*kx
            index as:         S3(ti,k,tj)

        2D
        Example
            name:             <none>_s
            how allocated:    as offsets into sm[SM_SIZE]
            dimensionality:   bx*by
            index as:         S2(ti,tj)

#endif




//#include "hd_block_size.h"

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


// basic indexing macros. indices are always given as global indices
// in undecompsed Domain(ids:ide,jds:jde)
//
// That is, given IJ (global index), the global Index mapped to
// a local index on a Patch(0:nx-1,0:ny-1) in Device Memory as:
//
// I - (ips-ims) + nx * ( J - (jps-jms) )
//
// where ips is the global index of the start of the patch (the -1 is
// for translating from WRF fortran indices).
//
// The global index I is mapped to a local index on a GPU Block's
// shared memory (0:bx-1, 0:by-1) as:
//
// I - (ips-ims) - bi * bx  +  by * ( J - (jps-jms) - bj * by )
//
// Where bi is the index into the GPU Block, and bx is the
// GPU Block Width.

// global to patch index converter
#define GtoP(i,p,P)      ((i)-(p)+(P))
#define GtoB(i,n,N,p,P)  ((i)-(p)+(P)-(n)*(N))

// thread index to local memory index = i + bi * bx + ips - ims
#define TtoP(i,a,b,c,d)             ((i)+(a)*(b)+(c)-(d))

//#define MAX(x,y) ((x)>(y)?(x):(y))
//#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) max(x,y)
#define MIN(x,y) min(x,y)

// basic indexing macros
//#define I2(i,j,m) ((i)+((j)*(m)))
//#define I3(i,j,m,k,n) (I2(i,j,m)+((k)*(m)*(n)))
#define I2(i,j,m) ((i)+(__mul24((j),(m))))
#define I3(i,j,m,k,n) (I2(i,j,m)+(__mul24((k),__mul24((m),(n)))))


// rgualan - modified macros
#define P2(i,j)    (bi*bx+i + (bj*by+j)*ix )
#define P3(i,k,j)  ((bi*bx+i) + k*ix + (bj*by+j)*ix*kx)

// index into a patch stored on device memory - 1
#if 0
//#define P2(i,j)    I2(TtoP(i,bi,bx,ips,ims),TtoP(j,bj,by,jps,jms),ime-ims+1)
//#define P3(i,k,j)  I3(TtoP(i,bi,bx,ips,ims),k,ime-ims+1,TtoP(j,bj,by,jps,jms),kme-kms+1)
#endif
// index into a block stored on shared memory
//#define S2(i,j)    (i + (j)*(bx))
//# define S3(i,k,j)  I3(i,k,bx,j,kme-kms+1)
//# define S3(i,k,j)  I3(i,j,bx,k,by)
//#define S3(i,k,j)  I3(k,i,kx,j,bx)

#define S2(i,j)  ((i+1)+((j+1)*(XXX+2)))
//#define S3(i,k,j)  ((i+1)+((j+1)*(XXX+2))+((k)*(XXX+2)*(YYY+2)))
#define S3(i,k,j)  ((i+1)+((j+1)*(XXX+2)))

// Local arrays in device mem
#define LOCDM(a,s) float * a ; cudaMalloc( (void**) & a , (s)*sizeof(float)) ;
// Local scratch arrays in shared memory
#define LOCSM(a,s) __shared__ float * a ; a = &(sm[isize]) ; isize += (s) ;
#define ig (TtoP(ti,bi,bx,ips,ims))
#define jg (TtoP(tj,bj,by,jps,jms))


#define ii (bi*bx+ti)
#define ij (bj*by+tj)





