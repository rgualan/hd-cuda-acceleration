typedef struct {
	// J dimension for processing
	int sub_dim_j;
	int sub_js;
	int sub_je;

	int data_js;
	int data_je;

	//float *h_Data;

	//Partial sum for this GPU
	//float *h_Sum;

	//Device buffers
	//float *d_Data,*d_Sum;

	//Reduction copied back from GPU
	//float *h_Sum_from_device;

	//Stream for asynchronous command execution
	//cudaStream_t stream;

} TGPUplan;

/*extern "C" void launch_reduceKernel(float *d_Result, float *d_Input, int N,
 int BLOCK_N, int THREAD_N, cudaStream_t &s);*/
