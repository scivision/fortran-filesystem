program test_pathlib

use pathlib, only : path_t, file_name, join, stem, suffix, root, get_cwd, &
is_absolute, with_suffix, relative_to, is_dir, sys_posix, exists, filesep, parent

implicit none (type, external)


call test_setter_getter()
print *, "OK: getter setter"

call test_join()
print *, "OK: test_join"

call test_filesep()
print *, "OK: pathlib: filesep"

call test_root()
print *, "OK: pathlib: root"

call test_manip()
print *, "OK: pathlib: manip"

call test_is_dir()
print *," OK: pathlib: is_dir"

call test_absolute()
print *, "OK: pathlib: absolute"

contains


subroutine test_setter_getter()

type(path_t) :: p1

p1 = path_t("a/b/c")

if (p1%path(2,3) /= "/b") error stop "getter start,end"
if (p1%path(3,3) /= "b") error stop "getter same"
if (p1%path(2) /= "/b/c") error stop "getter start only"

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

end subroutine test_join


subroutine test_filesep()

type(path_t) :: p1, p2, p3

if(sys_posix()) then
  if (filesep() /= "/") error stop "filesep posix"
else
  if(filesep() /= char(92)) error stop "filesep windows"
endif

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

end subroutine test_filesep


subroutine test_manip()

type(path_t) :: p1, p2

!> stem

if(stem("") /= "") error stop "stem empty"

p1 = path_t("hi.a.b")
if (p1%stem() /= "hi.a") error stop "stem failed"
p2 = path_t(p1%stem())
if (p2%stem() /= "hi") error stop "stem nest failed"
p2 = path_t("hi")
if (p2%stem() /= "hi") error stop "stem idempotent failed"

!> suffix

if(suffix("") /= "") error stop "suffix empty"

if (p1%suffix() /= ".b") error stop "suffix failed"
p2 = path_t(p1%suffix())
if (p2%suffix() /= "") error stop "suffix nest failed on " // p2%path()
p2 = path_t(p2%suffix())
if (p2%suffix() /= "") error stop "suffix idempotent failed"

!> parent

if(parent("") /= ".") error stop "parent empty: " // parent("")

p1 = path_t("a/b/c")
if (p1%parent() /= "a/b") error stop "parent failed" // p1%path()
p2 = path_t(p1%parent())
if (p2%parent() /= "a") error stop "parent nest failed" // p1%path()
p2 = path_t("a")
if (p2%parent() /= ".") error stop "parent idempotent failed. Expected '.', but got: " // p2%path()

p1 = path_t("a/b/c")
p2 = path_t("a")
if (p1%file_name() /= "c") error stop "file_name failed"
if (p2%file_name() /= "a") error stop "file_name idempotent failed"

p1 = path_t("my/file.h5")
p2 = p1%with_suffix(".hdf5")

if (p2%path() /= "my/file.hdf5") error stop "%with_suffix failed: " // p2%path()
if (p2%path() /= with_suffix("my/file.h5", ".hdf5")) error stop "with_suffix() failed: " // p2%path()

end subroutine test_manip


subroutine test_root()

type(path_t) :: p1, p2
character(:), allocatable :: r

if(root("") /= "") error stop "root empty"

p1 = path_t("/etc")
p2 = path_t("c:/etc")

if(sys_posix()) then
  r = p1%root()
  if(r /= "/") error stop "unix %root failed 1: " // r

  r = p2%root()
  if(r /= "") error stop "unix %root failed 2: " // r

  r = root("/etc")
  if(r /= "/") error stop "unix root() failed: " // r
else
  if(p1%root() == "/") error stop "windows %root failed"

  r = p2%root()
  if( r/= "c:") error stop "windows %root failed 2: " // r
  if(root("c:/etc") /= "c:") error stop "windows root() failed"
endif

end subroutine test_root


subroutine test_is_dir()

character(:), allocatable :: r

integer :: i

type(path_t) :: p1,p2,p3

if(is_dir("")) error stop "is_dir empty should be false"

if(sys_posix()) then
  if(.not. is_dir("/")) error stop "is_dir('/') failed"
else
  r = root(get_cwd())
  print *, "root drive: ", r
  if(.not. is_dir(r)) error stop "is_dir('" // r // "') failed"
endif

p1 = path_t(".")

if(.not. p1%is_dir()) error stop "did not detect '.' as directory"
if(p1%is_file()) error stop "detected '.' as file"

p2 = path_t('test-pathlib.h5')
open(newunit=i, file=p2%path(), status='replace')
close(i)

if (p2%is_dir()) error stop "detected file as directory"
call p2%remove()

p3 = path_t("not-exist-dir")
if(p3%is_dir()) error stop "not-exist-dir should not exist"

end subroutine test_is_dir


subroutine test_absolute()

type(path_t) :: p1,p2

p1 = path_t("")
if (p1%is_absolute()) error stop "blank is not absolute"

if (sys_posix()) then
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

end subroutine test_absolute


end program
