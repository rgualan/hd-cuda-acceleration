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

##################### cuda location ##############
CUDALIBPATH =  /usr/local/cuda/lib64

##################################################
NVCC	= nvcc
PHASE   = -cuda
NVOPT   = -O4 -Xptxas="-v" -use_fast_math -arch=sm_20


##################################################
.SUFFIXES :

all : hd_vanilla.exe


hd.cu.o : hd.cu
	$(NVCC) $(NVOPT) -o hd.cu.o -c hd.cu 

hd_gpu.cu.o : hd_gpu.cu
	$(NVCC) $(NVOPT) -o hd_gpu.cu.o -c hd_gpu.cu 

microclock.o : microclock.c
	$(CC) -c $(CFLAGS) microclock.c



%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $< 
	
hd_vanilla.exe: module_hd_serial.o hd_host.o hd_gpu.o util.o
	$(CC) -o $@ $^ 

clean :
	\rm -f *.o *.mod *.cpp *.exe
	