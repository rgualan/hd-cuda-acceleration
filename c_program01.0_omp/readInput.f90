program read_input
        !Parameters
        INTEGER ::  ids,ide, jds,jde, kds,kde , &
            ims,ime, jms,jme, kms,kme , &
            its,ite, jts,jte, kts,kte

        !Local variables
        logical :: cf_specified
        logical :: cf_nested
        logical :: cf_open_xs
        logical :: cf_open_xe
        logical :: cf_open_ys
        logical :: cf_open_ye
        logical :: cf_periodic_x
        logical :: cf_polar

        CHARACTER(LEN=1) :: name

        REAL , DIMENSION( -4:430, 1:35, -4:305 ) :: field, xkmhd
        REAL , DIMENSION( -4:430, 1:35, -4:305 ) :: tendency
        REAL , DIMENSION( -4:430, 1:35, -4:305 ) :: tendency_target
        REAL , DIMENSION( -4:430, 1:35, -4:305 ) :: tendency_input

        REAL , DIMENSION( -4:430, -4:305 ) :: mu

        REAL , DIMENSION( -4:430, -4:305 ) ::    msfux,      &
            msfuy,      &
            msfvx,      &
            msfvx_inv,  &
            msfvy,      &
            msftx,      &
            msfty

        REAL ::     rdx,       &
            rdy,       &
            khdif

        CHARACTER(LEN=50) :: in_fname, out_fname

        integer i,j,k
        integer counter
        integer countDiff
        integer countCalc

        real t_start, t_end
        real temp
        real maxError

        ! Read data from input file
        call cpu_time(t_start)
        in_fname = "inputdata-00001X"
        print *, "Reading input variables from file: ", in_fname
        open(unit = 100, file = in_fname)
        read(100,*) ids, ide, jds, jde, kds, kde
        read(100,*) ims, ime, jms, jme, kms, kme
        read(100,*) its, ite, jts, jte, kts, kte
        read(100,*) cf_specified, cf_nested, &
            cf_open_xs, cf_open_xe, &
            cf_open_ys, cf_open_ye, &
            cf_periodic_x, cf_polar
        read(100,*) name
        read(100,*) field
        read(100,*) tendency
        read(100,*) mu
        read(100,*) msfux, msfuy, msfvx, msfvx_inv, msfvy, msftx, msfty
        read(100,*) khdif, xkmhd, rdx, rdy
        close(100)
        call cpu_time(t_end)
        print '("Time for reading input variables = ",f6.3," seconds.")',t_end-t_start

        !Read variables
        print *, 'Read variables:'
        print *, 'Index d: ', ids, ide, jds, jde, kds, kde
        print *, 'Index m: ', ims, ime, jms, jme, kms, kme
        print *, 'Index t: ', its, ite, jts, jte, kts, kte
        print *, 'Boolean: ', cf_specified, cf_nested, &
            cf_open_xs, cf_open_xe, &
            cf_open_ys, cf_open_ye, &
            cf_periodic_x, cf_polar
        print *, 'Name: ', name
        print *, 'Real: ', khdif, rdx, rdy


        !Tendency input
        print *,'Field:'
        print *, field(1:100,10,10)
        
    stop
end program read_input
