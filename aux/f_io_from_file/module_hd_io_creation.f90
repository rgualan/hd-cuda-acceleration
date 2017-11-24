MODULE module_big_step_utilities_em

    USE module_configure, ONLY : grid_config_rec_type

    IMPLICIT NONE
    INTEGER :: general_counter = 0

CONTAINS

    SUBROUTINE horizontal_diffusion ( name, field, tendency, mu,           &
        config_flags,                        &
        msfux, msfuy, msfvx, msfvx_inv,      &
        msfvy, msftx, msfty,                 &
        khdif, xkmhd, rdx, rdy,              &
        ids, ide, jds, jde, kds, kde,        &
        ims, ime, jms, jme, kms, kme,        &
        its, ite, jts, jte, kts, kte        )

        !IMPLICIT NONE

        TYPE(grid_config_rec_type), INTENT(IN   ) :: config_flags

        INTEGER ,        INTENT(IN   ) :: ids, ide, jds, jde, kds, kde, &
            ims, ime, jms, jme, kms, kme, &
            its, ite, jts, jte, kts, kte

        CHARACTER(LEN=1) ,                          INTENT(IN   ) :: name

        REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) , INTENT(IN   ) :: field, xkmhd

        REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) , INTENT(INOUT) :: tendency

        REAL , DIMENSION( ims:ime , jms:jme ) , INTENT(IN   ) :: mu

        REAL , DIMENSION( ims:ime , jms:jme ) ,         INTENT(IN   ) :: msfux,      &
            msfuy,      &
            msfvx,      &
            msfvx_inv,  &
            msfvy,      &
            msftx,      &
            msfty

        REAL ,                                      INTENT(IN   ) :: rdx,       &
            rdy,       &
            khdif


        print *, "Exporting input variables of the method horizontal_diffusion!"
        open(unit = 100, file = "inputdata2")
        write(100,*) name
        write(100,*) field
        write(100,*) tendency
        write(100,*) mu
        write(100,*) config_flags
        write(100,*) msfux, msfuy, msfvx, msfvx_inv, msfvy, msftx, msfty
        write(100,*) khdif, xkmhd, rdx, rdy
        write(100,*) ids, ide, jds, jde, kds, kde
        write(100,*) ims, ime, jms, jme, kms, kme
        write(100,*) its, ite, jts, jte, kts, kte
        close(100)

        print *, "Exporting output variable of the method horizontal_diffusion!"
        open(unit = 200, file = "outputdata")
        write(200,*) tendency
        close(200)


    end subroutine horizontal_diffusion


    subroutine simple_test()
        IMPLICIT NONE

        general_counter = general_counter + 1
        print *, general_counter

    end subroutine simple_test


END MODULE module_big_step_utilities_em
