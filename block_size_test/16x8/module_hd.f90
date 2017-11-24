MODULE module_horizontal_diffusion

!USE module_nc

contains

    subroutine horizontal_diffusion( &
        ids,ide, jds,jde, kds,kde, &
        ims,ime, jms,jme, kms,kme, &
        its,ite, jts,jte, kts,kte )
        IMPLICIT NONE

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

        REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) :: field, xkmhd
        REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) :: tendency
        REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) :: tendency_target
        REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) :: tendency_input

        REAL , DIMENSION( ims:ime , jms:jme ) :: mu

        REAL , DIMENSION( ims:ime , jms:jme ) ::    msfux,      &
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


        !Call init gpu function
        print *, 'Calling horizontal_diffusion_gpu_init'
        call horizontal_diffusion_gpu_init(0,1,0)

        !Tendency input
!        print *,'Tendency input:'
!        print *, tendency(100:120,1,200)

        !Copy tendency for assesment purpouses
        tendency_input = tendency

        !Call host gpu funciton
        print *, 'Calling horizontal_diffusion_host'
        call horizontal_diffusion_host(ids, ide, jds, jde, kds, kde, &
            ims, ime, jms, jme, kms, kme, &
            its, ite, jts, jte, kts, kte, &
            cf_specified, cf_nested, &
            cf_open_xs, cf_open_xe, cf_open_ys, cf_open_ye, &
            cf_periodic_x, cf_polar, &
            name, &
            field, &
            tendency, &
            mu, &
            msfux, &
            msfuy, &
            msfvx, &
            msfvx_inv, &
            msfvy, msftx, msfty, &
            khdif, &
            xkmhd, &
            rdx, rdy)
        print *, 'Back from horizontal_diffusion_host'

        !Assesing output
        print *, 'Assesing output...'
        call cpu_time(t_start)
        out_fname = "outputdata-00001"
        print *, "Reading output variable from file: ", out_fname
        open(unit = 101, file = out_fname)
        read(101,*) tendency_target
        close(101)
        call cpu_time(t_end)
        print '("Time for reading output variable = ",f6.3," seconds.")',t_end-t_start
        !        print *,'Tendency output-target:'
        !        print *, tendency_target(100:120,1,200)
        !        print *, tendency_target(20:25,35,10)
        !        print *,'Tendency output-gpu:'
        !        print *, tendency(100:120,1,200)
        !        print *, tendency(20:25,35,10)

        !Asses calculation differences between CPU and GPU
!        open(unit = 900, file = 'debug.txt')
!        write(900,*) 'I, K, J, Input, Target, GPU, Diff'
        countDiff = 0
        maxError = 0
!
        DO k = kds, kde
            DO j = jds, jde
                DO i = ids, ide
                    IF( ABS( tendency_target(i,k,j) - tendency(i,k,j) ) > 1.0 ) THEN
                        countDiff = countDiff + 1
!                        write(900,*) i,k,j, tendency_input(i,k,j), tendency_target(i,k,j), tendency(i,k,j), ABS(tendency_target(i,k,j)-tendency(i,k,j))
                    ENDIF
                    !Evaluate max error
                    if (ABS(tendency_target(i,k,j)-tendency(i,k,j)) > maxError) THEN
                        maxError = ABS(tendency_target(i,k,j)-tendency(i,k,j))
                    ENDIF
                ENDDO
            ENDDO
        ENDDO
!        close(900)

        print *,'Big differences between target-tendency and gpu-tendency: ', countDiff, '/', ((ime-ims+1)*(kme-kms+1)*(jme-jms+1))
        !print *,'Calculated values: ', countCalc
        !print *,'Non-calculated values: ', (((ime-ims+1)*(kme-kms+1)*(jme-jms+1)) - countCalc)
        print *,'Max point-to-point error: ', maxError

        !Export output snapshot for comparissons
        print *,'Creating snapshots...'
        call create_snapshot('outputdataGPU.txt', tendency, ims,ime,kms,kme,jms,jme)
        call create_snapshot('outputdataNormal.txt', tendency_target, ims,ime,kms,kme,jms,jme)
        call create_snapshot('outputdataInput.txt', tendency_input, ims,ime,kms,kme,jms,jme)

    END SUBROUTINE horizontal_diffusion

    subroutine create_snapshot(file_name, var, ims,ime,kms,kme,jms,jme)
        CHARACTER(LEN=*) :: file_name
        INTEGER :: ims, ime, jms, jme, kms, kme
        REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) :: var

        !I/O
        open(unit=200, file=file_name, ACTION="write", STATUS="replace")
        !do k=kms,kme
        do j=jms,jme
            write(200, '(435F10.2)')( var(i,1,j) ,i=ims,ime)
        end do
        !end do
        close(200)

    end subroutine create_snapshot


END MODULE module_horizontal_diffusion

program horizontal_diffusion_driver
    use module_horizontal_diffusion
    IMPLICIT NONE

    INTEGER ::   ids, ide, jds, jde, kds, kde, &
        ims, ime, jms, jme, kms, kme, &
        its, ite, jts, jte, kts, kte

    CHARACTER(LEN=80) fname

    print *, "Running program horizontal_diffusion_driver..."

    ! Read data from input file
    fname = "inputdata-00001X"
    print*,'Opening ',trim(fname)
    open(unit = 100, file = fname)
    read(100,*) ids, ide, jds, jde, kds, kde
    read(100,*) ims, ime, jms, jme, kms, kme
    read(100,*) its, ite, jts, jte, kts, kte
    close(100)

    CALL horizontal_diffusion(ids,ide,jds,jde,kds,kde, &
        ims,ime,jms,jme,kms,kme, &
        its,ite,jts,jte,kts,kte)

    stop
end program horizontal_diffusion_driver
