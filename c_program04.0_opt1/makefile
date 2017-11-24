SHELL 	= sh

### GNU compilers ################################
CC      = gcc
FC      = gfortran
#CFLAGS  = -w -O3 -c
#CFLAGS  = -g -O3 -w -ftree-vectorize -ftree-loop-linear -funroll-loops -lm
CFLAGS  = -O3 -ftree-vectorize -ftree-loop-linear -funroll-loops -lm 
#FCFLAGS = -fconvert=big-endian -frecord-marker=4 -ffree-form
#FCFLAGS = -g -O3 -w -ftree-vectorize -ftree-loop-linear -funroll-loops
CFLAGS = -O3 -ftree-vectorize -ftree-loop-linear -funroll-loops
LD      = $(FC)
LDOPT   =

##################################################
#XXX = 16
#YYY = 16
#MKX = 35 
##################### cuda location ##############
CUDALIBPATH =  /usr/local/cuda-6.5/lib64
LIBCUBLAS = $(CUDALIBPATH)/libcublas.so
#LIBCUBLASEMU = $(CUDALIBPATH)/libcublasemu.so


##################################################
NVCC	= nvcc
PHASE   = -cuda
#NCOPT   = -g -O3 -I/home/ronald/lib/netcdf-4.1.3/include -L/home/ronald/lib/netcdf-4.1.3/lib -lnetcdff -lnetcdf
#NVOPT   = --host-compilation 'C++' --use_fast_math
#NVOPT   = -DXXX=$(XXX) -DYYY=$(YYY) -DMKX=$(MKX) --host-compilation 'C++' --use_fast_math
#NVOPT   = -DXXX=$(XXX) -DYYY=$(YYY) -DMKX=$(MKX) -g
#Options taken from paper of 2014
#NVOPT   = -DXXX=$(XXX) -DYYY=$(YYY) -DMKX=$(MKX) -g --ptxas-options=-v -arch=sm_21 --restrict --maxrregcount 63 -Xptxas -dlcm=ca -O3 -fmad=false
#NVOPT   = -DXXX=$(XXX) -DYYY=$(YYY) -DMKX=$(MKX) -g -arch=sm_21 -O3
#NVOPT   = -arch=sm_21 #-O3
#NVOPT   = -lineinfo -g -arch=sm_21 -O3
#NVOPT   = -O4 -Xptxas -dlcm=ca -maxrregcount=63 -use_fast_math -arch=sm_20 
#NVOPT   = -lineinfo -g -O4 -maxrregcount=41 -Xptxas="-v" -use_fast_math -arch=sm_20
#NVOPT   = -O4 -Xptxas="-v" -use_fast_math -arch=sm_20
NVOPT   = -O4 -Xptxas="-v" -arch=sm_20

.SUFFIXES :

all : chocolate

hd.cu.o : hd.cu
	$(NVCC) $(NVOPT) -o hd.cu.o -c hd.cu 

hd_gpu.cu.o : hd_gpu.cu
	$(NVCC) $(NVOPT) -o hd_gpu.cu.o -c hd_gpu.cu 

microclock.o : microclock.c
	$(CC) -c $(CFLAGS) microclock.c

chocolate : module_hd.f90 hd.cu.o hd_gpu.cu.o microclock.o 
	$(FC) -c $(FCFLAGS) module_hd.f90
	$(LD) -o hd_driver.exe $(LDOPT) module_hd.o hd.cu.o hd_gpu.cu.o -L$(CUDALIBPATH) -lcudart -lstdc++	

clean :
	\rm -f *.o *.mod *.cpp *.exe
