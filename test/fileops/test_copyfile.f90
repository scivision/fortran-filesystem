program test_copy

use filesystem

implicit none

call test_copyfile()
print '(a)', "OK: copy_file"

contains

subroutine test_copyfile()

type(path_t) :: p1, p2
integer :: u
logical :: ok

!> create dummy file
p1 = path_t('test-filesystem.h5')
open(newunit=u, file=p1%path(), status='replace')
close(u)
if(.not. p1%is_file()) error stop "did not detect " // p1%path() // " as file"

!> copy a file
p2 = path_t('test-filesystem.h5.copy')
call p1%copy_file(p2%path(), overwrite=.true., ok=ok)
if(.not. p2%is_file()) error stop "did not detect " // p2%path() // " as file"
if(.not. ok) error stop "copy_file ok logical is false despite success of copy " // p2%path()

!> empty target
! omitted because this fails when ABI shaky e.g. macOS with Clang+Gfortran
! call copy_file(p1%path(), "", status=i)
! if(i==0) error stop "copy_file should fail on empty target"

end subroutine test_copyfile

end program
