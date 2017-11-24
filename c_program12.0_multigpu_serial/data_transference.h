#define TODEV(A,s) float *A##_d;cudaMalloc((void**)&A##_d,((s))*sizeof(float));cudaMemcpy(A##_d,A,(s)*sizeof(float),cudaMemcpyHostToDevice);
#define FROMDEV(A,s) cudaMemcpy(A,A##_d,(s)*sizeof(float),cudaMemcpyDeviceToHost);
#define CLNUP(A) cudaFree(A##_d)

#define TODEV3(A) TODEV(A,d3)
#define TODEV2(A) TODEV(A,d2)
#define FROMDEV3(A) FROMDEV(A,d3)
#define FROMDEV2(A) FROMDEV(A,d2)
