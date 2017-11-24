typedef struct {
	//GPU id
	int gpu_id;

	// J dimension for processing
	int sub_dim_j;
	int sub_js;
	int sub_je;

	int data_js;
	int data_je;

	// Indices and dimensions for loading data from/to orginal variable (host-pageable memory)
	// For sending
	int s3; //Starting index for sending data 3D to device
	int s2; //Starting index for sending data 2D to device
	int d3; //Dimension of sent data
	int d2; //Dimension of sent data
	// For reading back
	int s3_; //Starting index for reading 3D data back
	int s2_; //Starting index for reading 2D data back
	int d3_; //Dimension of returning data
	int d2_; //Dimension of returning data
	int delta; // Delta of starting point for copy-back for ignoring extra-layer

	// Host (pinned) data
	float *field_h;
	float *tendency_h;
	float *xkmhd_h;
	float *mu_h;
	float *msfux_h;
	float *msfuy_h;
	float *msfvx_h;
	float *msfvx_inv_h;
	float *msfvy_h;
	float *msftx_h;
	float *msfty_h;

	// Device data
	float *field_d;
	float *tendency_d;
	float *xkmhd_d;
	float *mu_d;
	float *msfux_d;
	float *msfuy_d;
	float *msfvx_d;
	float *msfvx_inv_d;
	float *msfvy_d;
	float *msftx_d;
	float *msfty_d;

	//Stream for asynchronous command execution
	cudaStream_t stream;

} TGPUplan;

