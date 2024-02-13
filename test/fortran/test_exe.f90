program test_exe

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use filesystem

implicit none

character(9) :: perm
logical :: ok
integer :: i

character(:), allocatable :: e1, e2

allocate(character(get_max_path()) :: e1, e2)

if(command_argument_count() /= 2) error stop "specify <exe> <noexe>"
call get_command_argument(1, e1, status=i)
if(i/=0) error stop "ERROR:test_exe: get_command_argument(1) failed"
call get_command_argument(2, e2, status=i)
if(i/=0) error stop "ERROR:test_exe: get_command_argument(2) failed"

call test_not_exist()
print '(a)', "PASSED: test_not_exist"

call test_exist(e1, e2)
print '(a)', "PASSED: test_exist"

call test_chmod()
print '(a)', "PASSED: test_chmod"

call test_which()
print '(a)', "PASSED: test_which()"

contains

subroutine test_not_exist()

type(path_t) :: p1

!> empty file
if(is_exe("")) error stop "ERROR:test_exe: is_exe('') should be false"
if(len_trim(get_permissions("")) /= 0) error stop "ERROR:test_exe: get_permissions('') should be empty"

!> not exist file
p1 = path_t("not-exist-file")
if (p1%is_file()) error stop "ERROR:test_exe: not-exist-file should not exist."
if (p1%is_exe()) error stop "ERROR:test_exe: non-exist-file cannot be executable"
if(len_trim(get_permissions("not-exist-file")) /= 0) error stop "ERROR:test_exe: get_permissions('not-exist') should be empty"

end subroutine test_not_exist


subroutine test_exist(exe, noexe)

character(*), intent(in) :: exe, noexe

character(9) :: pe,pn

if(is_exe(parent(exe))) then
  write(stderr, '(a)') "ERROR:test_exe: directory" // parent(exe) // " should not be executable"
  error stop
endif

pe = get_permissions(exe)
print '(a)', "permissions: " // trim(exe) // " = " // pe

if (.not. is_file(exe)) then
  write(stderr,'(a)') "ERROR:test_exe: " // trim(exe) // " is not a file."
  error stop 77
endif

if (.not. is_exe(exe)) then
  write(stderr,'(a)') "ERROR:test_exe: " // trim(exe) // " is not executable."
  error stop
endif

pn = get_permissions(noexe)
print '(a)', "permissions: " // trim(noexe) // " = " // pn

if(.not. is_file(noexe)) then
  write(stderr,'(a)') "ERROR:test_exe: " // trim(noexe) // " is not a file."
  error stop 77
endif

if (is_exe(noexe)) then
  if(is_windows()) then
    write(stderr,'(a)') "XFAIL:test_exe: " // trim(noexe) // " is executable on Windows."
  else
    write(stderr,'(a)') "ERROR:test_exe: " // trim(noexe) // " is executable and should not be."
    error stop
  endif
endif

end subroutine test_exist


subroutine test_chmod()

type(path_t) :: p1, p2

character(:), allocatable :: exe, noexe

exe = join(get_tempdir(), "yes_exe")
noexe = join(get_tempdir(), "no_exe")

!> chmod(.true.)

p1 = path_t(exe)
p1 = p1%canonical()
call p1%touch()
if(.not. p1%is_file()) error stop "ERROR:test_exe: " // p1%path() // " is not a file."

perm = get_permissions(exe)
print '(a)', "permissions before chmod(true) " // p1%path() // " = " // perm

call chmod_exe(p1%path(), .true.)

call chmod_exe(p1%path(), .false., ok)
if (.not. ok) error stop "ERROR:test_exe: %chmod_exe(.true.) failed"

call p1%chmod_exe(.true.)


perm = get_permissions(exe)
print '(a)', "permissions after chmod(true): " // p1%path() // " = " // perm

if (.not. p1%is_exe()) then
  write(stderr,'(a)') "ERROR:test_exe: %is_exe() did not detect executable file " // trim(exe)
  if(.not. is_windows()) error stop
endif

if (.not. is_exe(p1%path())) then
  write(stderr, '(a)') "ERROR:test_exe: is_exe(path) did not detect executable file " // trim(exe)
  if(.not. is_windows()) error stop
endif

if(.not. is_windows()) then
if(perm(3:3) /= "x") then
  write(stderr,'(a)') "ERROR:test_exe: get_permissions() " // trim(exe) // " is not executable"
  error stop
endif
endif

!> chmod(.false.)

p2 = path_t(noexe)
call p2%touch()
if(.not. p2%is_file()) then
  write(stderr,'(a)') "ERROR:test_exe: " // trim(noexe) // " is not a file."
  error stop
endif

perm = get_permissions(noexe)
print '(a)', "permissions: " // trim(noexe) // " = " // perm

call p2%chmod_exe(.false., ok)
if (.not. ok) error stop "ERROR:test_exe: %chmod_exe(.false.) failed"

if(.not. is_windows()) then
!~ Windows file system is always executable to stdlib.

  if (p2%is_exe()) error stop "ERROR:test_exe: did not detect non-executable file."

  if(perm(3:3) /= "-") then
    write(stderr,'(a)') "ERROR:test_exe: get_permissions() " // trim(noexe) // " is executable"
    error stop
  endif

endif

end subroutine test_chmod


subroutine test_which()


character(:), allocatable :: buf

if (is_windows()) then
  buf = which("cmd.exe")
else
  buf = which("ls")
endif

print '(a)', "which: " // buf

if (len_trim(buf) == 0) error stop "ERROR:test_exe: which() failed"

end subroutine test_which

end program
