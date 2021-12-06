!! a CLI frontent for pathlib
!! helps people understand pathlib output

program cli

use pathlib

implicit none (type, external)

integer :: i
character(1000) :: buf
character(16) :: fcn

if (command_argument_count() < 2) error stop "usage: ./pathlib <function> [<path> ...]"

call get_command_argument(1, fcn, status=i)
if (i /= 0) error stop "invalid function name: " // trim(fcn)

call get_command_argument(2, buf, status=i)
if (i /= 0) error stop "invalid path> " // trim(buf)

select case (fcn)
case ("as_posix")
  print *, "as_posix: " // trim(buf), " ", trim(as_posix(buf))
case ("expanduser")
  print *, "expanduser: " // trim(buf), " ", trim(expanduser(buf))
case ("is_dir")
  print *, "is_dir: " // trim(buf), is_dir(buf)
case ("is_file")
  print *, "is_file: " // trim(buf), is_file(buf)
case ("resolve")
  print *, "resolve: " // trim(buf), " ", trim(resolve(buf))
case default
  error stop "unknown function> " // trim(fcn)
end select


end program
