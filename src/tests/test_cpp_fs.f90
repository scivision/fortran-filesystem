program test_cpp_fs
!! test methods from C++17 filesystem

use filesystem, only : path_t, get_cwd, exists, sys_posix, get_tempdir, get_homedir, match

implicit none (type, external)

call test_exists()
print *, "OK fs: exists"

call test_tempdir()
print *, "OK: tempdir, homedir"

call test_match()
print *, "OK: match"

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


subroutine test_match()

type(path_t) :: p

if(.not. match("abc", "abc")) error stop "match exact failed"
if(.not. match("abc", "a.*")) error stop "match wildcard failed"

if(.not. match("/abc", "a.c")) error stop "match() dot failed"
p = path_t("/abc")
if(.not. p%match("a.c")) error stop "%match dot failed"

if(.not. match("abc34v", "a.c\d{2}")) error stop "match decimal failed"

end subroutine test_match


end program
