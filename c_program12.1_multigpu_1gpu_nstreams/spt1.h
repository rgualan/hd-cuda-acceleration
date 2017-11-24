
// Min/Max functions
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

// Indexing a "multi-dimensional array" to a array
#define P2(i,j)    ((i) + (j)*(ix) )
#define P3(i,k,j)  ((i) + (k)*(ix) + (j)*(ix)*(kx))

// Convert Fortran index to C index - and to subdimain index
#define I(fi) (fi+4)
#define J(fj) (fj+4)
#define K(fk) (fk-1)

// Indexing for "partial arrays"
#define J2(j) ((j)-data_js)
