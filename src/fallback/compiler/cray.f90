submodule (filesystem) cray_no_cpp_fs
!! Cray external procedures
!! Cray Fortran has stat() subroutine but unsure of API.

use, intrinsic :: iso_fortran_env, only : int8

implicit none (type, external)


contains

module procedure f_unlink
external :: unlink
call unlink(path)
end procedure f_unlink


module procedure get_cwd
external :: getcwd

character(MAXP) :: work

call getcwd(work)
get_cwd = trim(work)
end procedure get_cwd


module procedure is_dir

integer :: u, ierr
integer(int8) :: x
character(:), allocatable :: wk

is_dir = .false.

wk = expanduser(path)
if(len_trim(wk) == 0) return

inquire(file=wk, exist=is_dir)
if(.not.is_dir) return

!> heuristic: try to open, and read. If fail, assume it's a directory
!> this has incorrect corner cases, but it's a start
open(newunit=u, file=wk, action='read', access='stream', status='old', iostat=ierr)
is_dir = ierr /= 0
if(is_dir) return
read(u, iostat=ierr) x
is_dir = ierr /= 0
close(u)

end procedure is_dir


module procedure file_size
file_size = 0
error stop "filesystem: %file_size() is not yet a feature for Cray"
end procedure file_size



end submodule cray_no_cpp_fs
