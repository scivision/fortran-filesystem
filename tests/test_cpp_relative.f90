program cpp_relative_to

use filesystem, only : path_t, relative_to, sys_posix, filesystem_has_normalize, filesystem_has_relative_to

implicit none (type, external)


call test_normal()
print *, "OK: normal full"

call test_relative_to
print *, "OK: relative_to full"

contains


subroutine test_normal()

type(path_t) :: p1, p2

if(.not. filesystem_has_normalize()) stop "filesystem: no normalize due to legacy C++ filesystem"

p1 = path_t("a//b/../c")
p2 = p1%normal()
if (p2%path() /= "a/c") error stop "normalize failed: " // p2%path()

end subroutine test_normal


subroutine test_relative_to()

type(path_t) :: p1
character(:), allocatable :: rel

if(.not. filesystem_has_relative_to()) stop "filesystem: no relative_to due to legacy C++ filesystem"

rel = relative_to("/", "")
if(rel /= "") error stop "empty base should be empty: " // rel

rel = relative_to("", "")
if(rel /= "") error stop "empty path and base should be empty: " // rel

rel = relative_to("", "/")
if(rel /= "") error stop "empty path should be empty: " // rel

print *, "OK: relative_to: empty"

if(sys_posix()) then
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
else
  rel  = relative_to("c:/a/b", "c")
  if(rel /= "") error stop "abs path with rel base should be empty: " // rel

  rel = relative_to("c", "c:/a/b")
  if(rel /= "") error stop "rel path with abs base should be empty: " // rel

  rel = relative_to("c:/a/b", "c:/a/b")
  if(rel /= ".") error stop "same path should be . "  // rel

  rel = relative_to("c:/a/b", "c:/a")
  if(rel /= "b") error stop "rel to parent 1: " // rel

  rel = relative_to("c:/a/b/c/d", "c:/a/b")
  if(rel /= "c/d") error stop "rel to parent 2: " // rel

  p1 = path_t("c:/a/b/c/d")
  if (p1%relative_to("c:/a/b") /= rel) error stop " OO rel to parent"

endif

end subroutine test_relative_to


end program
