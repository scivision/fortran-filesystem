program test_pathlib

use pathlib, only : path_t, file_name, join, stem, suffix, root, is_absolute, with_suffix, relative_to

implicit none (type, external)


call test_setter_getter()

call test_join()

call test_filesep()

call test_manip()

call test_is_dir()

call test_absolute()

call test_relative_to()

contains


subroutine test_setter_getter()

type(path_t) :: p1

p1 = path_t("a/b/c")

if (p1%path(2,3) /= "/b") error stop "getter start,end"
if (p1%path(3,3) /= "b") error stop "getter same"
if (p1%path(2) /= "/b/c") error stop "getter start only"

print *, "OK: getter setter"

end subroutine test_setter_getter


subroutine test_join()

type(path_t) :: p1,p2


p1 = path_t("a/b")

p2 = p1%join("c/d")
if (p2%path() /= "a/b/c/d") error stop "join"
p2 = p1%join("c/d/")
if (p2%path() /= "a/b/c/d/") error stop "join"
p2 = p1%join("c/d")
if (p2%path() /= "a/b/c/d") error stop "%join"
if (join("a/b", "c/d") /= "a/b/c/d") error stop "join()"

print *, "OK: test_join"

end subroutine test_join


subroutine test_filesep()

type(path_t) :: p1, p2, p3

p1 = path_t("")

p2 = p1%as_posix()
if (p2%path() /= "") error stop "as_posix empty"

p2 = p1%as_windows()
if (p2%path() /= "") error stop "as_windows empty"

p1 = path_t("/")
p3 = p1%as_posix()
if(p3%path() /= "/") error stop "as_posix '/' failed"

p2 = path_t(char(92))
p3 = p2%as_posix()
if(p3%path() /= "/") error stop "as_posix char(92) failed"

p3 = p1%as_windows()
if(p3%path() /= char(92)) error stop "as_windows '\' failed"

p3 = p2%as_windows()
if(p3%path() /= char(92)) error stop "as_windows char(92) failed"

print *, "OK: pathlib: filesep"

end subroutine test_filesep


subroutine test_manip()

type(path_t) :: p1, p2
logical :: is_unix

is_unix = is_absolute("/")

p1 = path_t("hi.a.b")
if (p1%stem() /= "hi.a") error stop "stem failed"
p2 = path_t(p1%stem())
if (p2%stem() /= "hi") error stop "stem nest failed"
p2 = path_t("hi")
if (p2%stem() /= "hi") error stop "stem idempotent failed"

if (p1%suffix() /= ".b") error stop "suffix failed"
p2 = path_t(p1%suffix())
if (p2%suffix() /= "") error stop "suffix nest failed on " // p2%path()
p2 = path_t(p2%suffix())
if (p2%suffix() /= "") error stop "suffix idempotent failed"

p1 = path_t("a/b/c")
if (p1%parent() /= "a/b") error stop "parent failed" // p1%path()
p2 = path_t(p1%parent())
if (p2%parent() /= "a") error stop "parent nest failed" // p1%path()
p2 = path_t("a")
if (p2%parent() /= ".") error stop "parent idempotent failed" // p2%path()

if (p1%file_name() /= "c") error stop "file_name failed"
if (p2%file_name() /= "a") error stop "file_name idempotent failed"

p1 = path_t("/etc")
p2 = path_t("c:/etc")
if(is_unix) then
  if(p1%root() /= "/") error stop "unix %root failed"
  if(p2%root() /= "") error stop "unix %root failed"
  if(root("/etc") /= "/") error stop "unix root() failed"
else
  if(p1%root() == "/") error stop "windows %root failed"
  if(p2%root() /= "c:") error stop "windows %root failed"
  if(root("c:/etc") /= "c:") error stop "windows root() failed"
endif

p1 = path_t("my/file.h5")
p2 = p1%with_suffix(".hdf5")

if (p2%path() /= "my/file.hdf5") error stop "%with_suffix failed: " // p2%path()
if (p2%path() /= with_suffix("my/file.h5", ".hdf5")) error stop "with_suffix() failed: " // p2%path()

print *, "OK: pathlib: manip"

end subroutine test_manip


subroutine test_is_dir()

integer :: i

type(path_t) :: p1,p2,p3

p1 = path_t(".")

if(.not. p1%is_dir()) error stop "did not detect '.' as directory"
if(p1%is_file()) error stop "detected '.' as file"

p2 = path_t('test-pathlib.h5')
open(newunit=i, file=p2%path(), status='replace')
close(i)

if (p2%is_dir()) error stop "detected file as directory"
call p2%unlink()

p3 = path_t("not-exist-dir")
if(p3%is_dir()) error stop "not-exist-dir should not exist"

print *," OK: pathlib: is_dir"

end subroutine test_is_dir


subroutine test_absolute()

type(path_t) :: p1,p2
logical :: is_unix

is_unix = is_absolute("/")

p1 = path_t("")
if (p1%is_absolute()) error stop "blank is not absolute"

if (is_unix) then
  p2 = path_t("/")
  if (.not. p2%is_absolute()) error stop p2%path() // "on Unix should be absolute"
  p2 = path_t("c:/")
  if (p2%is_absolute()) error stop p2%path() // "on Unix is not absolute"
else
  p2 = path_t("J:/")
  if (.not. p2%is_absolute()) error stop p2%path() // "on Windows should be absolute"
  p2 = path_t("j:/")
  if (.not. p2%is_absolute()) error stop p2%path() // "on Windows should be absolute"
  p2 = path_t("/")
  if (p2%is_absolute()) error stop p2%path() // "on Windows is not absolute"
endif

print *, "OK: pathlib: is_absolute"

end subroutine test_absolute


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

end subroutine

end program
