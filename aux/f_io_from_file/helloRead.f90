program Hello
    implicit none
    INTEGER :: i, j, k

    print *, "Reading variables from file..."

    open(unit = 2, file = "inputdata")

    read(2,*) i, j, k
    close(2)

    print *, "i,j,k: ", i, j, k


end program Hello
