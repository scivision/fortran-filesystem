program test_cpp_fs
!! test methods from C++17 filesystem

use pathlib, only : path_t, cwd, exists, relative_to

implicit none (type, external)

call test_exists()
print *, "OK fs: exists"

call test_relative_to()
print *, "OK fs: relative_to"

contains


subroutine test_exists()

type(path_t) :: p1

p1 = path_t(cwd())

if(.not. p1%exists()) error stop "%exists() failed"
if(.not. exists(cwd())) error stop "exists(cwd) failed"

end subroutine test_exists


subroutine test_relative_to()

type(path_t) :: p1
character(:), allocatable :: rel

rel = relative_to("/", "")
if(rel /= "") error stop "should be empty: " // rel

rel = relative_to("", "")
if(rel /= "") error stop "should be empty: " // rel

rel = relative_to("", "/")
if(rel /= "") error stop "should be empty: " // rel

rel = relative_to("/a", "b")
if(rel /= "") error stop "one abs, one rel should be empty: " // rel

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
