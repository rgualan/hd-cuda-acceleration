SHELL 	= sh

### GNU compilers ################################
#CC      = gcc
#CCFLAGS  = -O3 
#CCFLAGS  = -w -O3 -g -p -pg
#OMPFLAGS = -fopenmp

### Intel compilers ################################
CC      = icc
CCFLAGS  = -w -O0 -traceback -debug all -parallel-source-info=2
OMPFLAGS = -openmp

##################################################
.SUFFIXES :

all : executables

hdSerial.o hdOpenmpFor.o hdOpenmpManual.o : hdSerial.c hdOpenmpFor.c hdOpenmpManual.c
	$(CC) $(CCFLAGS) -o hdSerial.o -c hdSerial.c 
	$(CC) $(CCFLAGS) -o hdOpenmpFor.o -c hdOpenmpFor.c $(OMPFLAGS)
	$(CC) $(CCFLAGS) -o hdOpenmpManual.o -c hdOpenmpManual.c $(OMPFLAGS) 

module_hd_2.o : module_hd_2.c
	$(CC) $(CCFLAGS) -o module_hd_2.o -c module_hd_2.c

executables : module_hd_2.o hdSerial.o hdOpenmpFor.o hdOpenmpManual.o
	$(CC) $(CCFLAGS) -o hd_serial.exe module_hd_2.o hdSerial.o
	$(CC) $(CCFLAGS) -o hd_openmp_for.exe module_hd_2.o hdOpenmpFor.o $(OMPFLAGS)
	$(CC) $(CCFLAGS) -o hd_openmp_manual.exe module_hd_2.o hdOpenmpManual.o $(OMPFLAGS)

clean :
	/bin/rm -f *.o *.mod *.cpp *.exe

rerun0 : clean all
	./hd_serial.exe
	
rerun1 : clean all
	./hd_openmp_for.exe
	
rerun2 : clean all
	./hd_openmp_manual.exe	