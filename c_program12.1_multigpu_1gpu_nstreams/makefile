SHELL 	= /bin/sh

### GNU compilers ################################
CC      = gcc
#CCFLAGS  = -O3 
CCFLAGS  = -w -O3 -g -p -pg
#OMPFLAGS = -fopenmp

### Intel compilers ################################
#CC      = icc
#CCFLAGS  = -w -O0 -traceback -debug all -parallel-source-info=2
#CCFLAGS  = -w -O3
#OMPFLAGS = -openmp

##################### Cuda location ##############
CUDAPATH =  /usr/local/cuda
CUDALIBPATH =  ${CUDAPATH}/lib64
CUDASAMPLEINC = ${CUDAPATH}/samples/common/inc

##################################################
NVCC	= nvcc
PHASE   = -cuda
#NVOPT   = -m64 -O4 -Xptxas="-v" -use_fast_math -arch=sm_20 -w
NVOPT   = -G -g -O0 -m64 -Xptxas="-v" -use_fast_math -arch=sm_20 -w


##################################################
.SUFFIXES :

all : hd_chocolate.exe

hd_host_multi.o: hd_host_multi.cu
	$(NVCC) $(NVOPT) -I$(CUDASAMPLEINC) -o $@ -c $<

hd_gpu.o: hd_gpu.cu
	$(NVCC) $(NVOPT) -I$(CUDASAMPLEINC) -o $@ -c $<
	
%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $< 
	
hd_chocolate.exe: module_hd.o hd_host_multi.o hd_gpu.o 
	$(CC) -o $@ $^ -L$(CUDALIBPATH) -lcudart -lstdc++ 

clean :
	\rm -f *.o *.mod *.cpp *.exe
	