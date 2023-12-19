program test_exe

use filesystem, only : path_t, is_exe, get_permissions, is_windows, chmod_exe, which

implicit none

character(*), parameter :: exe_name = "dummy.exe", noexe_name = "dummy.no.exe"
character(9) :: perm
logical :: ok

call test_not_exist()
print '(a)', "PASSED: test_not_exist"

call test_exist()
print '(a)', "PASSED: test_exist"

call test_which()
print '(a)', "PASSED: test_which()"

contains

subroutine test_not_exist()

type(path_t) :: p1

if(is_exe("")) error stop "ERROR:test_exe: is_exe('') should be false"
if(len_trim(get_permissions("")) /= 0) error stop "ERROR:test_exe: get_permissions('') should be empty"

!> not exist file
p1 = path_t("not-exist-file")
if (p1%is_file()) error stop "ERROR:test_exe: not-exist-file should not exist."
if (p1%is_exe()) error stop "ERROR:test_exe: non-exist-file cannot be executable"
if(len_trim(get_permissions("not-exist-file")) /= 0) error stop "ERROR:test_exe: get_permissions('not-exist') should be empty"

end subroutine test_not_exist


subroutine test_exist()

type(path_t) :: p1, p2

!> chmod(.true.)

p1 = path_t(exe_name)
p1 = p1%canonical()
call p1%touch()
if(.not. p1%is_file()) error stop "ERROR:test_exe: " // p1%path() // " is not a file."

perm = get_permissions(exe_name)
print '(a)', "permissions before chmod(true) " // p1%path() // " = " // perm

call chmod_exe(p1%path(), .true.)

call chmod_exe(p1%path(), .false., ok)
if (.not. ok) error stop "ERROR:test_exe: %chmod_exe(.true.) failed"

call p1%chmod_exe(.true.)


perm = get_permissions(exe_name)
print '(a)', "permissions after chmod(true): " // p1%path() // " = " // perm

if (.not. p1%is_exe()) error stop "ERROR:test_exe: %is_exe() did not detect executable file " // exe_name
if (.not. is_exe(p1%path())) error stop "ERROR:test_exe: is_exe(path) did not detect executable file " // exe_name

if(.not. is_windows() .and. perm(3:3) /= "x") error stop "ERROR:test_exe: get_permissions() " // exe_name // " is not executable"

!> chmod(.false.)

p2 = path_t(noexe_name)
call p2%touch()
if(.not. p2%is_file()) error stop "ERROR:test_exe: " // noexe_name // " is not a file."

perm = get_permissions(noexe_name)
print '(a)', "permissions: " // noexe_name // " = " // perm

call p2%chmod_exe(.false., ok)
if (.not. ok) error stop "ERROR:test_exe: %chmod_exe(.false.) failed"

if(.not. is_windows()) then
!~ Windows file system is always executable to stdlib.

if (p2%is_exe()) error stop "ERROR:test_exe: did not detect non-executable file."

if(perm(3:3) /= "-") error stop "ERROR:test_exe: get_permissions() " // noexe_name // " is executable"
endif

end subroutine test_exist


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
