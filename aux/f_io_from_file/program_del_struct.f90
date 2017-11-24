program delete_config_flags_structure

USE module_configure, ONLY : grid_config_rec_type

    IMPLICIT NONE

    TYPE(grid_config_rec_type):: config_flags

    INTEGER ::   ids_, ide_, jds_, jde_, kds_, kde_, &
                 ims_, ime_, jms_, jme_, kms_, kme_, &
                 its_, ite_, jts_, jte_, kts_, kte_

    !Fue necesario quemar los datos en constantes
    !debido a que los arrays que se crean posteriormente usan estos indices
    INTEGER, PARAMETER ::   ids=1,  ide=425, jds=1,  jde=300, kds=1, kde=35, &
                            ims=-4, ime=430, jms=-4, jme=305, kms=1, kme=35, &
                            its=1,  ite=425, jts=1,  jte=300, kts=1, kte=35

    CHARACTER(LEN=1) :: name

    REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) :: field, xkmhd
    REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) :: tendency

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

    !Local
    INTEGER :: i, j, k, itf, jtf, ktf

    INTEGER :: i_start, i_end, j_start, j_end

    REAL :: mrdx, mkrdxm, mkrdxp, &
           mrdy, mkrdym, mkrdyp

    LOGICAL :: specified


    CHARACTER(LEN=100) :: input_file_name, output_file_name
    character(LEN=2) :: temp = "aa"


    !Timing
    real :: start_time, stop_time


     ! Leer los datos desde el archivo de entrada
     if(command_argument_count()==0)then
        print *, "Error. No input file was specified!"
        stop 9
     end if
     call get_command_argument(1,input_file_name)
     output_file_name = trim(adjustl(input_file_name))//trim(adjustl("_"))

    ! Leer los datos desde los archivos de entrada
    print *, "Reading input variables from file: ", input_file_name
    open(unit = 100, file = input_file_name)
    read(100,*) name
    read(100,*) field
    read(100,*) tendency
    read(100,*) mu
    read(100,*) config_flags
    read(100,*) msfux, msfuy, msfvx, msfvx_inv, msfvy, msftx, msfty
    read(100,*) khdif, xkmhd, rdx, rdy
    read(100,*) ids_, ide_, jds_, jde_, kds_, kde_
    read(100,*) ims_, ime_, jms_, jme_, kms_, kme_
    read(100,*) its_, ite_, jts_, jte_, kts_, kte_
    close(100)

    print *, "Exporting input variables to file: ", output_file_name
    open(unit = 101, file = output_file_name)
    write(101,*) ids, ide, jds, jde, kds, kde
    write(101,*) ims, ime, jms, jme, kms, kme
    write(101,*) its, ite, jts, jte, kts, kte
    write(101,*) config_flags%specified, config_flags%nested, &
        config_flags%open_xs, config_flags%open_xe, &
        config_flags%open_ys, config_flags%open_ye, &
        config_flags%periodic_x, config_flags%polar
    write(101,*) name
    write(101,*) field
    write(101,*) tendency
    write(101,*) mu
    write(101,*) msfux, msfuy, msfvx, msfvx_inv, msfvy, msftx, msfty
    write(101,*) khdif, xkmhd, rdx, rdy
    close(101)

end program delete_config_flags_structure
