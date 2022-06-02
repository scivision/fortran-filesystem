submodule (filesystem) cray_no_cpp_fs
!! Cray external procedures
!! Cray Fortran has stat() subroutine but unsure of API.

use, intrinsic :: iso_fortran_env, only : int8, iostat_end

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


module procedure file_size
file_size = 0
error stop "filesystem: %file_size() is not yet a feature for Cray"
end procedure file_size



end submodule cray_no_cpp_fs
