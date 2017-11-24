SHELL 	= sh

### GNU compilers ################################
CC      = gcc
FC      = gfortran
#CFLAGS  = -O3 -ftree-vectorize -ftree-loop-linear -funroll-loops -w
CFLAGS  = -O0 -g -p -pg
#FFLAGS  = -O3 -ftree-vectorize -ftree-loop-linear -funroll-loops 
FFLAGS  = -O0 -g -p -pg
LD      = $(CC)
LDOPT   =

##################### cuda location ##############
CUDALIBPATH =  /usr/local/cuda/lib64
LIBCUBLAS = $(CUDALIBPATH)/libcublas.so

##################################################
NVCC	= nvcc
PHASE   = -cuda
NVOPT   = -O4 -Xptxas="-v" -arch=sm_20

.SUFFIXES :

all : vanilla

readInput.exe: readInput.f90
	$(FC) -o readInput.exe readInput.f90;

hd.o : hd.c
	$(CC) $(CFLAGS) -o hd.o -c hd.c 

module_hd_2.o : module_hd_2.c
	$(CC) $(CFLAGS) -o module_hd_2.o -c module_hd_2.c

vanilla : module_hd_2.o hd.o
	$(CC) $(CFLAGS) -o hd_driver.exe module_hd_2.o hd.o	

clean :
	\rm -f *.o *.mod *.cpp *.exe
