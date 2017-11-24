!wrf:MODEL_LAYER:DYNAMICS
!

MODULE module_big_step_utilities_em2

   !USE module_model_constants
   !USE module_state_description, only: p_qg, p_qs, p_qi, gdscheme, tiedtkescheme, kfetascheme, g3scheme, &
   !gfscheme,p_qv, param_first_scalar, p_qr, p_qc, DFI_FWD
   USE module_configure, ONLY : grid_config_rec_type
   !use module_timing
   !USE module_wrf_error

   !RGualan - begin
   IMPLICIT NONE
   INTEGER :: function_counter = 0
   !RGualan - end

CONTAINS

!-------------------------------------------------------------------------------
SUBROUTINE horizontal_diffusion ( name, field, tendency, mu,           &
                                  config_flags,                        &
                                  msfux, msfuy, msfvx, msfvx_inv,      &
                                  msfvy, msftx, msfty,                 &
                                  khdif, xkmhd, rdx, rdy,              &
                                  ids, ide, jds, jde, kds, kde,        &
                                  ims, ime, jms, jme, kms, kme,        &
                                  its, ite, jts, jte, kts, kte        )

   IMPLICIT NONE

   ! Input data


   !grid_config_rec_type es una estructura que almacena datos sobre la configuración de
   !la simulación
   TYPE(grid_config_rec_type), INTENT(IN   ) :: config_flags

   !Dimensiones del dominio a procesar
   INTEGER ,        INTENT(IN   ) :: ids, ide, jds, jde, kds, kde, &
                                     ims, ime, jms, jme, kms, kme, &
                                     its, ite, jts, jte, kts, kte

   !name puede ser u, v o w
   !probablemente representa alguna direccion
   CHARACTER(LEN=1) ,                          INTENT(IN   ) :: name

   !arrays tridimensionales
   !indices de los arrays tridimensionales: x, z, y
   REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) , INTENT(IN   ) :: field, xkmhd
   !variable de salida!
   REAL , DIMENSION( ims:ime , kms:kme , jms:jme ) , INTENT(INOUT) :: tendency

   !arrays bidimensionales
   !indices: x, y
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

   ! Local data

   INTEGER :: i, j, k, itf, jtf, ktf

   INTEGER :: i_start, i_end, j_start, j_end

   REAL :: mrdx, mkrdxm, mkrdxp, &
           mrdy, mkrdym, mkrdyp

   LOGICAL :: specified


   CHARACTER(LEN=50) :: file_name

!<DESCRIPTION>
!
!  horizontal_diffusion computes the horizontal diffusion tendency
!  on model horizontal coordinate surfaces.
!
!</DESCRIPTION>

    !2014-Nov Rgualan (Begin)
!    function_counter = function_counter + 1
!    print *, "Exporting input variables of the method horizontal_diffusion!"
!    WRITE(file_name,*) "inputdata-", function_counter
!    open(unit = 100, file = file_name)
!    write(100,*) name
!    write(100,*) field
!    write(100,*) tendency
!    write(100,*) mu
!    write(100,*) config_flags
!    write(100,*) msfux, msfuy, msfvx, msfvx_inv, msfvy, msftx, msfty
!    write(100,*) khdif, xkmhd, rdx, rdy
!    write(100,*) ids, ide, jds, jde, kds, kde
!    write(100,*) ims, ime, jms, jme, kms, kme
!    write(100,*) its, ite, jts, jte, kts, kte
!    close(100)
    !2014-Nov Rgualan (End)
!call start_timing


   specified = .false.
   if(config_flags%specified .or. config_flags%nested) specified = .true.

   ktf=MIN(kte,kde-1)

   IF (name .EQ. 'u') THEN

      i_start = its
      i_end   = ite
      j_start = jts
      j_end   = MIN(jte,jde-1)

      IF ( config_flags%open_xs .or. specified ) i_start = MAX(ids+1,its)
      IF ( config_flags%open_xe .or. specified ) i_end   = MIN(ide-1,ite)
      IF ( config_flags%open_ys .or. specified ) j_start = MAX(jds+1,jts)
      IF ( config_flags%open_ye .or. specified ) j_end   = MIN(jde-2,jte)
      IF ( config_flags%periodic_x ) i_start = its
      IF ( config_flags%periodic_x ) i_end = ite


      DO j = j_start, j_end
      DO k=kts,ktf
      DO i = i_start, i_end

         ! The interior is grad: (m_x*d/dx), the exterior is div: (m_x*m_y*d/dx(/m_y))
         ! setting up different averagings of m^2 partial d/dX and m^2 partial d/dY

         mkrdxm=(msftx(i-1,j)/msfty(i-1,j))*mu(i-1,j)*xkmhd(i-1,k,j)*rdx
         mkrdxp=(msftx(i,j)/msfty(i,j))*mu(i,j)*xkmhd(i,k,j)*rdx
         mrdx=msfux(i,j)*msfuy(i,j)*rdx
         mkrdym=( (msfuy(i,j)+msfuy(i,j-1))/(msfux(i,j)+msfux(i,j-1)) )* &
                0.25*(mu(i,j)+mu(i,j-1)+mu(i-1,j-1)+mu(i-1,j))* &
                0.25*(xkmhd(i,k,j)+xkmhd(i,k,j-1)+xkmhd(i-1,k,j-1)+xkmhd(i-1,k,j))*rdy
         mkrdyp=( (msfuy(i,j)+msfuy(i,j+1))/(msfux(i,j)+msfux(i,j+1)) )* &
                0.25*(mu(i,j)+mu(i,j+1)+mu(i-1,j+1)+mu(i-1,j))* &
                0.25*(xkmhd(i,k,j)+xkmhd(i,k,j+1)+xkmhd(i-1,k,j+1)+xkmhd(i-1,k,j))*rdy
         ! need to do four-corners (t) for diffusion coefficient as there are
         ! no values at u,v points
         ! msfuy - has to be y as part of d/dY
         !         has to be u as we're at a u point
         mrdy=msfux(i,j)*msfuy(i,j)*rdy

         ! correctly averaged version of rho~ * m^2 *
         !    [partial d/dX(partial du^/dX) + partial d/dY(partial du^/dY)]
            tendency(i,k,j)=tendency(i,k,j)+( &
                            mrdx*(mkrdxp*(field(i+1,k,j)-field(i  ,k,j))  &
                                 -mkrdxm*(field(i  ,k,j)-field(i-1,k,j))) &
                           +mrdy*(mkrdyp*(field(i,k,j+1)-field(i,k,j  ))  &
                                 -mkrdym*(field(i,k,j  )-field(i,k,j-1))))
      ENDDO
      ENDDO
      ENDDO

   ELSE IF (name .EQ. 'v')THEN

      i_start = its
      i_end   = MIN(ite,ide-1)
      j_start = jts
      j_end   = jte

      IF ( config_flags%open_xs .or. specified ) i_start = MAX(ids+1,its)
      IF ( config_flags%open_xe .or. specified ) i_end   = MIN(ide-2,ite)
      IF ( config_flags%open_ys .or. specified ) j_start = MAX(jds+1,jts)
      IF ( config_flags%open_ye .or. specified ) j_end   = MIN(jde-1,jte)
      IF ( config_flags%periodic_x ) i_start = its
      IF ( config_flags%periodic_x ) i_end = MIN(ite,ide-1)
      IF ( config_flags%polar ) j_start = MAX(jds+1,jts)
      IF ( config_flags%polar ) j_end   = MIN(jde-1,jte)

      DO j = j_start, j_end
      DO k=kts,ktf
      DO i = i_start, i_end

         mkrdxm=( (msfvx(i,j)+msfvx(i-1,j))/(msfvy(i,j)+msfvy(i-1,j)) )*    &
                0.25*(mu(i,j)+mu(i,j-1)+mu(i-1,j-1)+mu(i-1,j))* &
                0.25*(xkmhd(i,k,j)+xkmhd(i,k,j-1)+xkmhd(i-1,k,j-1)+xkmhd(i-1,k,j))*rdx
         mkrdxp=( (msfvx(i,j)+msfvx(i+1,j))/(msfvy(i,j)+msfvy(i+1,j)) )*    &
                0.25*(mu(i,j)+mu(i,j-1)+mu(i+1,j-1)+mu(i+1,j))* &
                0.25*(xkmhd(i,k,j)+xkmhd(i,k,j-1)+xkmhd(i+1,k,j-1)+xkmhd(i+1,k,j))*rdx
         mrdx=msfvx(i,j)*msfvy(i,j)*rdx
         mkrdym=(msfty(i,j-1)/msftx(i,j-1))*xkmhd(i,k,j-1)*rdy
         mkrdyp=(msfty(i,j)/msftx(i,j))*xkmhd(i,k,j)*rdy
         mrdy=msfvx(i,j)*msfvy(i,j)*rdy

            tendency(i,k,j)=tendency(i,k,j)+( &
                            mrdx*(mkrdxp*(field(i+1,k,j)-field(i  ,k,j))  &
                                 -mkrdxm*(field(i  ,k,j)-field(i-1,k,j))) &
                           +mrdy*(mkrdyp*(field(i,k,j+1)-field(i,k,j  ))  &
                                 -mkrdym*(field(i,k,j  )-field(i,k,j-1))))
      ENDDO
      ENDDO
      ENDDO

   ELSE IF (name .EQ. 'w')THEN

      i_start = its
      i_end   = MIN(ite,ide-1)
      j_start = jts
      j_end   = MIN(jte,jde-1)

      IF ( config_flags%open_xs .or. specified ) i_start = MAX(ids+1,its)
      IF ( config_flags%open_xe .or. specified ) i_end   = MIN(ide-2,ite)
      IF ( config_flags%open_ys .or. specified ) j_start = MAX(jds+1,jts)
      IF ( config_flags%open_ye .or. specified ) j_end   = MIN(jde-2,jte)
      IF ( config_flags%periodic_x ) i_start = its
      IF ( config_flags%periodic_x ) i_end = MIN(ite,ide-1)

      DO j = j_start, j_end
      DO k=kts+1,ktf
      DO i = i_start, i_end

         mkrdxm=(msfux(i,j)/msfuy(i,j))*   &
                0.25*(mu(i,j)+mu(i-1,j)+mu(i,j)+mu(i-1,j))* &
                0.25*(xkmhd(i,k,j)+xkmhd(i-1,k,j)+xkmhd(i,k-1,j)+xkmhd(i-1,k-1,j))*rdx
         mkrdxp=(msfux(i+1,j)/msfuy(i+1,j))*   &
                0.25*(mu(i+1,j)+mu(i,j)+mu(i+1,j)+mu(i,j))* &
                0.25*(xkmhd(i+1,k,j)+xkmhd(i,k,j)+xkmhd(i+1,k-1,j)+xkmhd(i,k-1,j))*rdx
         mrdx=msftx(i,j)*msfty(i,j)*rdx
!         mkrdym=(msfvy(i,j)/msfvx(i,j))*   &
         mkrdym=(msfvy(i,j)*msfvx_inv(i,j))*   &
                0.25*(mu(i,j)+mu(i,j-1)+mu(i,j)+mu(i,j-1))* &
                0.25*(xkmhd(i,k,j)+xkmhd(i,k,j-1)+xkmhd(i,k-1,j)+xkmhd(i,k-1,j-1))*rdy
!         mkrdyp=(msfvy(i,j+1)/msfvx(i,j+1))*   &
         mkrdyp=(msfvy(i,j+1)*msfvx_inv(i,j+1))*   &
                0.25*(mu(i,j+1)+mu(i,j)+mu(i,j+1)+mu(i,j))* &
                0.25*(xkmhd(i,k,j+1)+xkmhd(i,k,j)+xkmhd(i,k-1,j+1)+xkmhd(i,k-1,j))*rdy
         mrdy=msftx(i,j)*msfty(i,j)*rdy

            tendency(i,k,j)=tendency(i,k,j)+( &
                            mrdx*(mkrdxp*(field(i+1,k,j)-field(i  ,k,j)) &
                                 -mkrdxm*(field(i  ,k,j)-field(i-1,k,j))) &
                           +mrdy*(mkrdyp*(field(i,k,j+1)-field(i,k,j  )) &
                                 -mkrdym*(field(i,k,j  )-field(i,k,j-1))))
      ENDDO
      ENDDO
      ENDDO

   ELSE


      i_start = its
      i_end   = MIN(ite,ide-1)
      j_start = jts
      j_end   = MIN(jte,jde-1)

      IF ( config_flags%open_xs .or. specified ) i_start = MAX(ids+1,its)
      IF ( config_flags%open_xe .or. specified ) i_end   = MIN(ide-2,ite)
      IF ( config_flags%open_ys .or. specified ) j_start = MAX(jds+1,jts)
      IF ( config_flags%open_ye .or. specified ) j_end   = MIN(jde-2,jte)
      IF ( config_flags%periodic_x ) i_start = its
      IF ( config_flags%periodic_x ) i_end = MIN(ite,ide-1)

      DO j = j_start, j_end
      DO k=kts,ktf
      DO i = i_start, i_end

         mkrdxm=(msfux(i,j)/msfuy(i,j))*0.5*(xkmhd(i,k,j)+xkmhd(i-1,k,j))*0.5*(mu(i,j)+mu(i-1,j))*rdx
         mkrdxp=(msfux(i+1,j)/msfuy(i+1,j))*0.5*(xkmhd(i+1,k,j)+xkmhd(i,k,j))*0.5*(mu(i+1,j)+mu(i,j))*rdx
         mrdx=msftx(i,j)*msfty(i,j)*rdx
!         mkrdym=(msfvy(i,j)/msfvx(i,j))*0.5*(xkmhd(i,k,j)+xkmhd(i,k,j-1))*0.5*(mu(i,j)+mu(i,j-1))*rdy
         mkrdym=(msfvy(i,j)*msfvx_inv(i,j))*0.5*(xkmhd(i,k,j)+xkmhd(i,k,j-1))*0.5*(mu(i,j)+mu(i,j-1))*rdy
!         mkrdyp=(msfvy(i,j+1)/msfvx(i,j+1))*0.5*(xkmhd(i,k,j+1)+xkmhd(i,k,j))*0.5*(mu(i,j+1)+mu(i,j))*rdy
         mkrdyp=(msfvy(i,j+1)*msfvx_inv(i,j+1))*0.5*(xkmhd(i,k,j+1)+xkmhd(i,k,j))*0.5*(mu(i,j+1)+mu(i,j))*rdy
         mrdy=msftx(i,j)*msfty(i,j)*rdy

            tendency(i,k,j)=tendency(i,k,j)+( &
                            mrdx*(mkrdxp*(field(i+1,k,j)-field(i  ,k,j))  &
                                 -mkrdxm*(field(i  ,k,j)-field(i-1,k,j))) &
                           +mrdy*(mkrdyp*(field(i,k,j+1)-field(i,k,j  ))  &
                                 -mkrdym*(field(i,k,j  )-field(i,k,j-1))))
      ENDDO
      ENDDO
      ENDDO

   ENDIF

    !2014-Nov RGualan (Begin)
!    print *, "Exporting output variable of the method horizontal_diffusion!"
!    WRITE(file_name,*) "outputdata-", function_counter
!    open(unit = 200, file = file_name)
!    write(200,*) tendency
!    close(200)
    !2014-Nov RGualan (End)


    !CALL end_timing ( 'Hola' )
END SUBROUTINE horizontal_diffusion

!-------------------------------------------------------------------------------
END MODULE module_big_step_utilities_em2
