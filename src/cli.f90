!! a CLI frontent for pathlib
!! helps people understand pathlib output

program cli

use pathlib

implicit none (type, external)

integer :: i
character(1000) :: buf, buf2
character(16) :: fcn

if (command_argument_count() < 1) error stop "usage: ./pathlib <function> [<path> ...]"

call get_command_argument(1, fcn, status=i)
if (i /= 0) error stop "invalid function name: " // trim(fcn)

if (fcn /= "home") then
  call get_command_argument(2, buf, status=i)
  if (i /= 0) error stop "invalid path: " // trim(buf)
endif

select case (fcn)
case ("same_file", "with_suffix")
  call get_command_argument(3, buf2, status=i)
  if (i /= 0) error stop "invalid path: " // trim(buf2)
end select

select case (fcn)
case ("as_posix")
  print *, "as_posix: " // trim(buf), " ", as_posix(buf)
case ("as_windows")
  print *, "as_windows: " // trim(buf), " ", as_windows(buf)
case ("drop_sep")
  print *, "drop_sep: " // trim(buf), " ", drop_sep(buf)
case ("expanduser")
  print *, "expanduser: " // trim(buf), " ", expanduser(buf)
case ("home")
  print *, "home: ", home()
case ("is_absolute")
  print *, "is_absolute: " // trim(buf), " ", is_absolute(buf)
case ("is_dir")
  print *, "is_dir: " // trim(buf), is_dir(buf)
case ("is_exe")
  print *, "is_exe: " // trim(buf), is_exe(buf)
case ("is_file")
  print *, "is_file: " // trim(buf), is_file(buf)
case ("mkdir")
  print *, "mkdir: " // trim(buf)
  call mkdir(trim(buf))
case ("parent")
  print *, "parent: " // trim(buf), " ", parent(buf)
case ("resolve")
  print *, "resolve: " // trim(buf), " ", resolve(buf)
case ("root")
  print *, "root: " // trim(buf), " ", root(buf)
case ("same_file")
  print *, "same_file: " // trim(buf) // " " // trim(buf2), " ", same_file(buf, buf2)
case ("size_bytes")
  print *, "size_bytes: " // trim(buf), size_bytes(buf)
case ("stem")
  print *, "stem: " // trim(buf), " ", stem(buf)
case ("suffix")
  print *, "suffix: " // trim(buf), " ", suffix(buf)
case ("with_suffix")
  print *, "with_suffix: " // trim(buf), " ", with_suffix(buf, buf2)
case default
  error stop "unknown function> " // trim(fcn)
end select


end program
