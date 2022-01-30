program test_cpp_fs
!! test methods from C++17 filesystem

use filesystem, only : path_t, get_cwd, exists, sys_posix, get_tempdir, get_homedir

implicit none (type, external)

call test_exists()
print *, "OK fs: exists"

call test_tempdir()
print *, "OK: tempdir, homedir"

contains


subroutine test_exists()

type(path_t) :: p1

if(exists("")) error stop "empty does not exist"

p1 = path_t(get_cwd())

if(.not. p1%exists()) error stop "%exists() failed"
if(.not. exists(get_cwd())) error stop "exists(get_cwd) failed"

end subroutine test_exists


subroutine test_tempdir()

character(:), allocatable :: temp, home


home = get_homedir()
if (len_trim(home) == 0) error stop "get_homedir failed"

temp = get_tempdir()
if (len_trim(temp) == 0) error stop "get_tempdir failed"

end subroutine test_tempdir


end program
