program test_cpp_fs
!! test methods from C++17 filesystem

use pathlib, only : path_t, get_cwd, exists, relative_to

implicit none (type, external)

call test_normal()
print *, "OK test_normal()"

call test_exists()
print *, "OK fs: exists"

call test_relative_to()
print *, "OK fs: relative_to"

contains


subroutine test_normal()

type(path_t) :: p1, p2

p1 = path_t("a//b/../c")
p2 = p1%normal()
if (p2%path() /= "a/c") error stop "normalize failed" // p2%path()

end subroutine test_normal


subroutine test_exists()

type(path_t) :: p1

if(exists("")) error stop "empty does not exist"

p1 = path_t(get_cwd())

if(.not. p1%exists()) error stop "%exists() failed"
if(.not. exists(get_cwd())) error stop "exists(get_cwd) failed"

end subroutine test_exists


subroutine test_relative_to()

type(path_t) :: p1
character(:), allocatable :: rel

rel = relative_to("/", "")
if(rel /= "") error stop "empty base should be empty: " // rel

rel = relative_to("", "")
if(rel /= "") error stop "empty path and base should be empty: " // rel

rel = relative_to("", "/")
if(rel /= "") error stop "empty path should be empty: " // rel

rel = relative_to("/a/b", "c")
if(rel /= "") error stop "abs path with rel base should be empty: " // rel

rel = relative_to("c", "/a/b")
if(rel /= "") error stop "rel path with abs base should be empty: " // rel

rel = relative_to("/a/b", "/a/b")
if(rel /= ".") error stop "same path should be . "  // rel

rel = relative_to("/a/b", "/a")
if(rel /= "b") error stop "rel to parent 1: " // rel

rel = relative_to("/a/b/c/d", "/a/b")
if(rel /= "c/d") error stop "rel to parent 2: " // rel

p1 = path_t("/a/b/c/d")
if (p1%relative_to("/a/b") /= rel) error stop " OO rel to parent"

end subroutine test_relative_to


end program
