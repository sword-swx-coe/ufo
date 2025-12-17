! (C) Copyright 2017-2018 UCAR
!
! This software is licensed under the terms of the Apache Licence Version 2.0
! which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.

!> Fortran module for GOLD_TDISK observation operator

module ufo_GOLD_TDISK_mod

 use oops_variables_mod
 use obs_variables_mod
 use ufo_vars_mod

 implicit none
 private

!> Fortran derived type for the observation type
! TODO: fill in if needed
 type, public :: ufo_GOLD_TDISK
 private
   type(obs_variables), public :: obsvars
   type(oops_variables), public :: geovars
 contains
   procedure :: setup  => ufo_GOLD_TDISK_setup
   procedure :: simobs => ufo_GOLD_TDISK_simobs
   final :: destructor
 end type ufo_GOLD_TDISK

contains

! ------------------------------------------------------------------------------
! TODO: add setup of your observation operator (optional)
subroutine ufo_GOLD_TDISK_setup(self, f_conf)
use fckit_configuration_module, only: fckit_configuration
implicit none
class(ufo_GOLD_TDISK), intent(inout)     :: self

! TODO: consider whether passing the Configuration object to this function
! is necessary. If only a small number of parameters are used,
! you could pass them in directly instead. In that case you can modify the
! interface appropriately.
type(fckit_configuration), intent(in) :: f_conf


! TODO: add input variables (requested from the model)
! self%geovars%push_back("variable name")
self%geovars%push_back("temperature")
end subroutine ufo_GOLD_TDISK_setup

! ------------------------------------------------------------------------------
! TODO: add cleanup of your observation operator (optional)
subroutine destructor(self)
implicit none
type(ufo_GOLD_TDISK), intent(inout) :: self

end subroutine destructor








SUBROUTINE loc_2_sza(lat,lon,utc,doy,sza)
  ! lat: -90 to 90deg.; lon: in decimal degrees, negative for west; doy=day of year; utc=universal time
    real(r8),        intent(in):: lat, lon, utc
    real(r8)                  :: declin, rad, theta_0, correct, angle, lat_rad, angle_rad, tmp1, tmp2, theta_rad
    integer,     intent(in):: doy
    real(r8),       intent(out):: sza


    rad=180.0_r8/3.14159265358979323846_r8
    theta_0 = 360.0_r8 * (dble(doy) - 1)/365.0_r8/rad

    declin  = 0.396372_r8 - 22.91327_r8* cos(theta_0) &
          + 4.02543_r8* sin(theta_0)               &
          - 0.387205_r8* cos(2.0*theta_0)          &
          + 0.051967_r8* sin(2.0*theta_0)          &
          - 0.154527_r8* cos(3.0*theta_0)          &
          + 0.084798_r8* sin(3.0*theta_0)


    declin = declin / rad   !;convert to radians

    ! ;***************************************************************;
    ! ; Time correction for solar hour angle                    ;
    ! ;***************************************************************;

    correct  = 0.004297_r8 + 0.107029_r8* cos(theta_0)  &
           - 1.837877_r8*sin(theta_0) - 0.837378_r8*cos(2.0*theta_0)  &
           - 2.342824_r8*sin(2.0*theta_0)

    angle = (utc - 12.0_r8) * 15.0_r8 + lon + correct
    if (angle .gt. 180.0_r8)  angle = angle - 360.0_r8
    if (angle .lt.-180.0_r8)  angle = angle + 360.0_r8


    lat_rad = lat/rad
    angle_rad  = angle/rad

    ! ;***************************************************************;
    ! ; Sun Zenith                                              ;
    ! ;***************************************************************;

    tmp1 = sin(lat_rad)*sin(declin) + cos(lat_rad)*cos(declin)*cos(angle_rad)
    tmp2   = abs(tmp1)


    if (tmp2 .gt. 1.1) then
        write(*,*) 'Error in acos argument in sun zenith'

    else if (tmp2 .gt. 1.0) then
            if (tmp1 .gt. 0.0) tmp1=1.0
            if (tmp1 .lt. 0.0) tmp1=-1.0
    end if
    theta_rad = acos(tmp1)

    !# ;***************************************************************;                                                ;
    !# ;        Convert to degrees                                      ;
    !# ;***************************************************************;

    sza = theta_rad * rad

    RETURN
    end subroutine loc_2_sza

SUBROUTINE lin_interp(x,y,x_n,xp,yp)
    integer,     intent(in)    :: x_n
    real(r8),        intent(in):: x(x_n), y(x_n), xp
    real(r8),       intent(out):: yp
    real(r8)                   :: x1,x2,y1,y2
    integer                    :: i


    do i=1,x_n-1
        if ((xp.gt.x(i)) .and. (xp.lt.x(i+1))) then ! for monotinacally increasing x arrays
            x1=x(i); x2=x(i+1)
            y1=y(i); y2=y(i+1)

        else if ((xp.lt.x(i)) .and. (xp.gt.x(i+1))) then ! for monotinacally decreasing x arrays
            x1=x(i); x2=x(i+1)
            y1=y(i); y2=y(i+1)

        else if  (xp.eq.x(i)) then  ! for xi==xp case
            x1=x(i); x2=x(i+1)
            y1=y(i); y2=y(i+1)
        endif
    end do

    yp=(y2-y1)/(x2-x1)*(xp-x2)+y2
    return
end subroutine lin_interp


subroutine get_expected_gold_temp(state_handle, ens_size, location, obs_time, Temps, istatus)

use quad_utils_mod, only : quad_lon_lat_locate,quad_interp_handle

type(ensemble_type), intent(in) :: state_handle
integer,             intent(in) :: ens_size
type(location_type), intent(in) :: location
type(time_type),     intent(in) :: obs_time
real(r8),           intent(out) :: expected_obs(ens_size)
integer,            intent(out) :: istatus(ens_size)

! varaibles inserted by FIL
!-------------------------
real(r8):: sza_obs(6), temp_out
real(r8), dimension(100,6) :: cf
real(r8), dimension(100) :: vc
real(r8), dimension(6,MAXLEVELS,ens_size):: CF_at_mc
real(r8), dimension(MAXLEVELS) :: ln_p_p0 ! cf at model
real(r8), dimension(6) :: sza_cf=(/-0.00001_r8,37.0_r8,53.0_r8,66.0_r8,78.0_r8,90.0_r8/)
real(r8)::xp, sza, lon, lat, utc, szw, tmp
integer  :: i, lev_i, li, n_vc=100, n_sza_cf=6, doy
integer   :: year, month, day, hour, minute, second
!-------------------------

! local variables
integer :: this_istatus(ens_size)
integer :: lev,nlev,iens
real(r8) :: loc_vals(3)
real(r8) :: amp,mu,sig,p_lev
real(r8) :: t_eff(ens_size),t(MAXLEVELS,ens_size),pressure(MAXLEVELS,ens_size)
real(r8) :: contrib_func(MAXLEVELS,ens_size)
type(location_type) :: location_lev
logical :: return_now

if (.not. module_initialized) call initialize_module


istatus = 0

loc_vals = get_location(location)

nlev = 0

! loop over levels and get temperature at the lat/lon of the observations
do lev=1,MAXLEVELS

  if (lev > MAXLEVELS ) then
     write(string1,'(''more than '',i4,'' levels in the model.'')') MAXLEVELS
     string2='increase MAXLEVELS in obs_def_GOLD_mod.f90, rerun preprocess and recompile.'
  endif 

  ! At each altitude interpolate the temperature to the lon-lat where data 
  ! point is located. 
  location_lev = set_location(loc_vals(1), loc_vals(2), real(lev, r8), VERTISLEVEL) 
  
  ! get temperature at this location
  QTY_TEMPERATURE = Temps[i]

  ! pressures
  QTY_PRESSURE = location[i]

enddo

! failed first time through loop - no values to return.
if (nlev == 0) then
   expected_obs(:) = MISSING_R8
   return
endif



!================================FIL====================
! READ CONTRIBUTION FUNCTION TEXT TILE; 7 COLS, 100 ROWS, FIRST COL=LOG(P/P_0); OTHER 6 COLS
!      corresponds to 6 CFs for 6 SZAs; defined as sza_cf 
open(1, file = 'CF_dat.txt', status='old')
do i = 1,100
    read(1,*) vc(i),cf(i,:)
end do
close(1)

!   do i = 1,100  
!      write(*,*)  vc(i),cf(i,6)
!   end do 
! call lin_interp(vc(:),cf(:,1),100,-21.03245, xp) ! Example run of lin_interp
! write(*,*) xp
! call loc_2_sza(-10.0,310.0,15.0,33,sza) ! Example run of SZA
! write(*,*) sza

! calculate ln(p/p0) for model levels
!do lev=1,nlev
!  p_lev = SUM(pressure(lev,:)) / dble(ens_size)
!  p_lev = p_lev / 100. ! convert Pa -> hPa
!  ln_p_p0(lev) = log(p_lev / 1013.0) ! ln(p/p0)
!end do


! loop to interpolate each SZA at all model levels and for all ensembles
do i=1,6
   do lev=1,nlev
      do iens=1,ens_size
         call lin_interp(vc(:),cf(:,i), n_vc, log(pressure(lev,iens)/101300_r8),temp_out) 
         CF_at_mc(i,lev,iens)= temp_out
      enddo
   enddo
enddo



lon=loc_vals(1)
lat=loc_vals(2)
call get_date(obs_time, year, month, day, hour, minute, second)
utc=dble(hour)+ dble(minute)/60.0_r8 + dble(second)/3600.0_r8

doy=julian_day(year,month,day)
!call ymd2ydoy(year,month,day,doy)

call loc_2_sza(lat,lon,utc,doy,szw) ! to output szw at lat, lon, utc, and doy of the obs.! utc is in hours

sza_cf(:)=(/-0.00001_r8,37.0_r8,53.0_r8,66.0_r8,78.0_r8,90.0_r8/)
contrib_func(:,:)=0.0      
do lev_i=2,nlev-1
   do iens=1,ens_size
      call lin_interp(sza_cf(:), CF_at_mc(:,lev_i,iens),n_sza_cf,szw,tmp)
      contrib_func(lev_i,iens)=tmp
   enddo
enddo
!==============================FIL==========================

!open(2, file = 'cfd.txt',action='write',position='append')
!write(2,*) contrib_func(:,1), szw, '   Next/'
!close(2)
!open(3, file = 'vc.txt',action='write',position='append')
!write(3,*) log(pressure(:,1)/101300_r8), utc, '   Next/'
!close(3)               


! calculate the effective temperature for each ensemble member
!t_eff(:) = 0.0
!do iens=1,ens_size
!  do lev=2,nlev-1
!    t_eff(iens) = t_eff(iens) + contrib_func(lev,iens)*t(lev,iens)
!  end do
!  t_eff(iens)=t_eff(iens)/SUM(contrib_func(:,iens))
!end do
t_eff(:) = 0.0
do iens=1,ens_size
    t_eff(iens) = SUM(contrib_func(:,iens)*t(:,iens))/SUM(contrib_func(:,iens))
end do

istatus = 0
expected_obs = t_eff
end subroutine get_expected_gold_temp
! ------------------------------------------------------------------------------
! TODO: put code for your nonlinear observation operator in this routine
! Code in this routine is for GOLD_TDISK only, please remove and replace
subroutine ufo_GOLD_TDISK_simobs(self, geovals, obss, nvars, nlocs, hofx)
use kinds
use ufo_geovals_mod, only: ufo_geovals, ufo_geoval, ufo_geovals_get_var
use iso_c_binding
use obsspace_mod
implicit none
class(ufo_GOLD_TDISK), intent(in) :: self
integer, intent(in)               :: nvars, nlocs
type(ufo_geovals),  intent(in)    :: geovals
real(c_double),     intent(inout) :: hofx(nvars, nlocs)
type(c_ptr), value, intent(in)    :: obss

! Local variables
!type(ufo_geoval), pointer :: geoval
!real(kind_real), dimension(:), allocatable :: obss_metadata

! check if some variable is in geovals and get it (var_tv is defined in ufo_vars_mod)
!call ufo_geovals_get_var(geovals, var_tv, geoval)

! get some metadata from obsspace
!allocate(obss_metadata(nlocs))
!call obsspace_get_db(obss, "MetaData", "some_metadata", obss_metadata)

! put observation operator code here

do i=0,nvars
    hofx[i] = get_expected_gold_temp(1, 1, geovals[i*nlocs:i*nlocs+nlocs], ufo_geovals_get_var(geovals[i*nlocs:i*nlocs+nlocs], var_tv, "UST")[0], ufo_geovals_get_var(geovals[i*nlocs:i*nlocs+nlocs], var_tv, "Temp"), 0)
enddo


end subroutine ufo_GOLD_TDISK_simobs


! ------------------------------------------------------------------------------

end module ufo_GOLD_TDISK_mod
