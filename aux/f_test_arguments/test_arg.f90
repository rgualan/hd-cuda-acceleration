program test_arg
 integer::narg,i !#of arg & counter of arg
 character(len=20)::name !Arg name

 !Check if any arguments are found
 narg=command_argument_count()

 !Loop over the arguments
 if(narg>0)then
 !loop across options
 do i=1,narg
  call get_command_argument(i,name)
  write(*,*)name
 end do
 end if
end program test_arg
