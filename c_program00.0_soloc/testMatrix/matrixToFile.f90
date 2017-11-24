program test
IMPLICIT NONE

INTEGER i,j,k
REAL, DIMENSION( 1:5 , 1:5 , 1:5 ) :: matrix
REAL counter

counter = 0

DO k = 1, 5
DO j = 1, 5
DO i = 1, 5
	!matrix(i,j,k) = counter
	!counter = counter + 1
	matrix(i,j,k) = j
ENDDO
ENDDO
ENDDO


print *, matrix(1:5,1:5,1:5)    

OPEN(UNIT=10, FILE="outf.txt", ACTION="write", STATUS="replace")
WRITE(10,*) matrix
CLOSE(UNIT=10)
 

stop
end program test
