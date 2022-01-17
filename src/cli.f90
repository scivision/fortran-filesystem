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

select case (fcn)
case ("cwd", "home", "tempdir")
case default
  if (command_argument_count() < 2) error stop "usage: ./pathlib <function> <path>"
  call get_command_argument(2, buf, status=i)
  if (i /= 0) error stop "invalid path: " // trim(buf)
end select

select case (fcn)
case ("same_file", "with_suffix")
  if (command_argument_count() < 3) error stop "usage: ./pathlib <function> <path> <path>"
  call get_command_argument(3, buf2, status=i)
  if (i /= 0) error stop "invalid path: " // trim(buf2)
end select

select case (fcn)
case ("as_posix")
  print '(A)', as_posix(buf)
case ("as_windows")
  print '(A)', as_windows(buf)
case ("cwd")
  print '(A)', trim(cwd())
case ("drop_sep")
  print '(A)', drop_sep(buf)
case ("expanduser")
  print '(A)', expanduser(buf)
case ("home")
  print '(A)', home()
case ("is_absolute")
  print '(L1)', is_absolute(buf)
case ("is_dir")
  print '(L1)', is_dir(buf)
case ("is_exe")
  print '(L1)', is_exe(buf)
case ("is_file")
  print '(L1)', is_file(buf)
case ("is_symlink")
  print '(L1)', is_symlink(buf)
case ("mkdir")
  print *, "mkdir: " // trim(buf)
  call mkdir(trim(buf))
case ("parent")
  print '(A)', parent(buf)
case ("resolve")
  print '(A)', resolve(buf)
case ("root")
  print '(A)', root(buf)
case ("same_file")
  print '(L1)', same_file(buf, buf2)
case ("size_bytes")
  print '(I0)', size_bytes(buf)
case ("stem")
  print '(A)', stem(buf)
case ("suffix")
  print '(A)', suffix(buf)
case ("tempdir")
  print '(A)', get_tempdir()
case ("with_suffix")
  print '(A)', with_suffix(buf, buf2)
case default
  error stop "unknown function> " // trim(fcn)
end select


end program
