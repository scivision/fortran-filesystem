program test_fileop

use, intrinsic:: iso_fortran_env, only : stderr=>error_unit

use filesystem, only : path_t, copy_file, is_absolute, get_cwd, normal, is_dir, &
mkdir, touch, copy_file, assert_is_file, get_max_path, set_cwd, canonical

implicit none

integer :: i
character(:), allocatable :: buf

allocate(character(get_max_path()) :: buf)

if(command_argument_count() < 1) error stop "please specify path to chdir"
call get_command_argument(1, buf, status=i)
if(i/=0) error stop "failed to get command argument"

call test_chdir(buf)
print '(a)', "OK: chdir set_cwd"

call test_touch()
print '(a)', "OK: touch"

call test_mkdir()
print '(a)', "OK: mkdir"

call test_copyfile()
print '(a)', "OK: copy_file"

deallocate(buf) !< valgrind

contains


subroutine test_chdir(path)
character(*), intent(in) :: path

logical :: ok

character(:), allocatable :: old_cwd, cwd, req

old_cwd = get_cwd()

print '(a)', 'current working directory: ', old_cwd

ok = set_cwd(path)

if (.not. ok) error stop "chdir failed"

cwd = get_cwd()
print '(a)', 'New working directory: ', cwd

req = canonical(path)

ok = set_cwd(old_cwd)
!! avoid messing up subsequent test location

if (cwd /= req) error stop "chdir failed: " // req // " != " // cwd

end subroutine


subroutine test_touch

type(path_t) :: p

! call touch("")  !< error stops

call touch("test_fileop.h5")

p = path_t("test_fileop.empty")
call p%touch()
if(.not. p%is_file()) error stop "touch failed"
call assert_is_file(p%path())

end subroutine test_touch


subroutine test_mkdir()

type(path_t) :: p

character(:), allocatable :: pwd, p2, p1

! call mkdir("")  !< error stops

pwd = get_cwd()

p1 = pwd // "/test-filesystem-dir1"
print '(a)', "mkdir: testing " // p1
call mkdir(p1)
if(.not. is_dir(p1)) error stop "mkdir: full: " // p1

print '(a)', "test that existing dir doesn't fail"
call mkdir(p1)

p = path_t("test-filesystem-dir/hello")
call p%mkdir()
if(.not.p%is_dir()) error stop "mkdir: single: " // p%path()

p2 = normal(pwd // "/test-filesystem-dir2/hello_posix")
print '(a)', "mkdir: testing " // p2
call mkdir(p2)
if(.not. is_dir(p2)) error stop "mkdir: full_posix: " // p2

end subroutine test_mkdir


subroutine test_copyfile()

type(path_t) :: p1, p2
integer :: u

!> create dummy file
p1 = path_t('test-filesystem.h5')
open(newunit=u, file=p1%path(), status='replace')
close(u)
if(.not. p1%is_file()) error stop "did not detect " // p1%path() // " as file"

!> copy a file
p2 = path_t('test-filesystem.h5.copy')
call p1%copy_file(p2%path(), overwrite=.true.)
if(.not. p2%is_file()) error stop "did not detect " // p2%path() // " as file"

!> empty target
! omitted because this fails when ABI shaky e.g. macOS with Clang+Gfortran
! call copy_file(p1%path(), "", status=i)
! if(i==0) error stop "copy_file should fail on empty target"

end subroutine test_copyfile


end program
