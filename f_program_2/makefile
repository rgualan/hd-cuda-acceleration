SHELL 	= sh

### Intel compilers
CC      = icc
FC      = ifort

### GNU compilers ################################
#CC      = gcc
#FC      = gfortran
CFLAGS  = -g -w -O3 -c 
#FCFLAGS = -fconvert=big-endian -frecord-marker=4 -ffree-form
#FCFLAGS = -g -O3 -w -ftree-vectorize -ftree-loop-linear -funroll-loops -lm
FCFLAGS = -pg -w 
OPT     =  -O3
LD      = $(FC)
LDOPT   =

# WRF-frame path
#WRFFRAME=/home/ronald/src/WRFV3.6.1_original_gnu/frame
WRFFRAME=/home/ronald/src/WRFV3.6.1_original_intel/frame

.SUFFIXES :

all : vanilla

hd_serial.exe : hd_serial.f90
#	$(FC) -c $(OPT) $(FCFLAGS) hd_serial.f90
#	$(LD) -o hd_serial.exe $(LDOPT) hd_serial.o
#	ifort -o hd_serial.exe hd_serial.f90 -I/home/ronald/src/WRFV3.6.1/frame/ 
#	$(FC) $(FCFLAGS) -o hd_serial.exe hd_serial.f90 -I/home/ronald/src/WRFV3.6.1_original_gnu/frame
	$(FC) $(FCFLAGS) -o hd_serial.exe hd_serial.f90 -I${WRFFRAME}

vanilla : hd_serial.exe

#vanilla : module_mp_wsm5.F libmassv.o microclock.o
#	/lib/cpp -P $(DEBUGDEBUG) $(DEVICEEMU) $(DEBUGOUTPUT) -DRWORDSIZE=$(RWORDSIZE) module_mp_wsm5.F > module_mp_wsm5_van.f90
#	$(FC) -c $(OPT) $(FCFLAGS) module_mp_wsm5_van.f90
#	$(LD) -o wsm5_driver_vanilla $(LDOPT) module_mp_wsm5_van.o libmassv.o microclock.o $(LIBCUBLASEMU)
#	cp wsm5_driver_vanilla run/

clean :
	\rm -f *.o *.mod *.cpp *.exe
