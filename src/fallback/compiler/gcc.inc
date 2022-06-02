submodule (filesystem) gcc_no_cpp_fs
!! GCC non-C++ filesystem

implicit none (type, external)

contains

module procedure f_unlink
intrinsic :: unlink
call unlink(path)
end procedure f_unlink


module procedure get_cwd

integer :: i
character(MAXP) :: work

i = getcwd(work)
if(i /= 0) error stop "filesystem:get_cwd: could not get current working dir"

get_cwd = as_posix(work)

end procedure get_cwd


end submodule gcc_no_cpp_fs
