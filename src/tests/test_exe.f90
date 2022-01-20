program test_exe

use pathlib, only : path_t, is_exe

implicit none (type, external)

type(path_t) :: p1
character(4096) :: buf
integer :: i

if(is_exe("")) error stop "is_ext('') should be false"

call get_command_argument(1, buf, status=i)
if (i/=0) error stop "test_executable: input path to an executable file"

p1 = path_t(trim(buf))
if (.not.p1%is_exe()) error stop "%is_exe did not detect executable file " // p1%path()
if (.not.is_exe(p1%path())) error stop "is_exe(path) did not detect executable file " // p1%path()

p1 = path_t("not-exist-file")
if (p1%is_exe()) error stop "non-existant file cannot be exectuable " // p1%path()

if(command_argument_count() == 2) then
  call get_command_argument(2, buf, status=i)
  if (i/=0) error stop "test_executable: input path to an non-executable file"

  p1 = path_t(trim(buf))
  if (p1%is_exe()) error stop "did not detect non-executable file " // p1%path()
else
  print *, "SKIP: non-exe test due to WSL"
endif

print *, "OK: pathlib: executable"

end program
