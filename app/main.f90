!! a CLI frontent for filesystem
!! helps people understand filesystem output

program cli

use filesystem

implicit none (type, external)

integer :: i
character(1000) :: buf, buf2
character(16) :: fcn

if (command_argument_count() < 1) error stop "usage: ./filesystem_cli <function> [<path> ...]"

call get_command_argument(1, fcn, status=i)
if (i /= 0) error stop "invalid function name: " // trim(fcn)

select case (fcn)
case ("get_cwd", "homedir", "tempdir", "is_unix", "is_linux", "is_windows", "is_macos", "max_path")
case default
  if (command_argument_count() < 2) error stop "usage: ./filesystem_cli <function> <path>"
  call get_command_argument(2, buf, status=i)
  if (i /= 0) error stop "invalid path: " // trim(buf)
end select

select case (fcn)
case ("relative_to", "same_file", "with_suffix")
  if (command_argument_count() < 3) error stop "usage: ./filesystem_cli <function> <path> <path>"
  call get_command_argument(3, buf2, status=i)
  if (i /= 0) error stop "invalid path: " // trim(buf2)
end select

select case (fcn)
case ('is_macos')
  print '(L1)', is_macos()
case ('is_windows')
  print '(L1)', is_windows()
case ('is_linux')
  print '(L1)', is_linux()
case ('is_unix')
  print '(L1)', is_unix()
case ("as_posix")
  print '(A)', as_posix(buf)
case ("get_cwd")
  print '(A)', trim(get_cwd())
case ("normal")
  print '(A)', normal(buf)
case ("expanduser")
  print '(A)', expanduser(buf)
case ("homedir")
  print '(A)', get_homedir()
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
case ("relative_to")
  print '(A)', relative_to(buf, buf2)
case ("resolve")
  print '(A)', resolve(buf)
case ("root")
  print '(A)', root(buf)
case ("same_file")
  print '(L1)', same_file(buf, buf2)
case ("file_size")
  print '(I0)', file_size(buf)
case ("stem")
  print '(A)', stem(buf)
case ("suffix")
  print '(A)', suffix(buf)
case ("tempdir")
  print '(A)', get_tempdir()
case ("with_suffix")
  print '(A)', with_suffix(buf, buf2)
case ("max_path")
  print '(i0)', get_max_path()
case default
  error stop "unknown function> " // trim(fcn)
end select


end program
