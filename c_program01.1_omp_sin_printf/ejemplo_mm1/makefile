SHELL 	= sh

### GNU compilers ################################
CC      = gcc
FC      = gfortran
CCFLAGS  = -O3 
FCFLAGS  = -O3  
LD      = $(CC)
LDOPT   =

##################### cuda location ##############
CUDALIBPATH =  /usr/local/cuda/lib64
LIBCUBLAS = $(CUDALIBPATH)/libcublas.so

##################################################
NVCC	= nvcc
PHASE   = -cuda
NVOPT   = -O3 -Xptxas="-v" -arch=sm_20

.SUFFIXES :

all : mm.exe mmO3.exe mmOmp.exe mmOmpO3.exe

mm.exe : mm.c
	$(CC) -o mm.exe mm.c 

mmO3.exe : mm.c
	$(CC) $(CCFLAGS) -o mmO3.exe mm.c 

mmOmp.exe : mm_openmp.c
	$(CC) -o mmOmp.exe mm_openmp.c -fopenmp 

mmOmpO3.exe : mm_openmp.c
	$(CC) $(CCFLAGS) -o mmOmpO3.exe mm_openmp.c -fopenmp 


run : all
	./run.sh
	
clean :
	/bin/rm -f *.o *.mod *.cpp *.exe
