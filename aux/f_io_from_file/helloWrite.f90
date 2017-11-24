program Hello
    implicit none
    INTEGER :: i, j, k

    print *, "Hello world!"

    i = 10
    j = 20
    k = 30

    open(unit = 2, file = "inputdata")

    write(2,*) i, j, k

    close(2)




end program Hello
