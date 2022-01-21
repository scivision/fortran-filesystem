program test_relative

use filesystem, only : path_t, relative_to

implicit none (type, external)

call test_relative_to()
print *, "OK: non-C++17 filesystem relative_to"

contains

subroutine test_relative_to()

type(path_t) :: p1
character(:), allocatable :: rel


if(relative_to("/", "") /= "") error stop "empty p2"
if(relative_to("/a", "b") /= "") error stop "one abs, one rel"
if(relative_to("/a/b", "/a/b") /= ".") error stop "same path"

rel = relative_to("/a/b", "/a")
if(rel /= "b") error stop "rel to parent 1: " // rel

rel = relative_to("/a/b/c/d", "/a/b")
if(rel /= "c/d") error stop "rel to parent 2: " // rel
p1 = path_t("/a/b/c/d")
if (p1%relative_to("/a/b") /= rel) error stop " OO rel to parent"

end subroutine test_relative_to

end program
