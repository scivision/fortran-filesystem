program test_filesystem

use filesystem, only : path_t, file_name, join, stem, suffix, root, get_cwd, &
is_absolute, with_suffix, relative_to, is_dir, sys_posix, exists, filesep, parent, &
assert_is_dir, as_posix

implicit none (type, external)


call test_setter_getter()
print *, "OK: getter setter"

call test_join()
print *, "OK: test_join"

call test_filesep()
print *, "OK: filesystem: filesep"

call test_stem()
print *, "OK: filesystem: stem"

call test_parent()
print *, "OK: filesystem: parent"

call test_filename()
print *, "OK: filesystem: filename"

call test_suffix()
print *, "OK: filesystem: suffix"

call test_with_suffix()
print *, "OK: filesystem: with_suffix"

call test_root()
print *, "OK: filesystem: root"

call test_is_dir()
print *, "OK: filesystem: is_dir"

call test_absolute()
print *, "OK: filesystem: absolute"

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
  if (filesep() /= "/") error stop "filesep posix: " // filesep()
else
  if(filesep() /= char(92)) error stop "filesep windows: " // filesep()
endif

p1 = path_t("")

p2 = p1%as_posix()
if (p2%path() /= "") error stop "as_posix empty"
if (as_posix("") /= "") error stop "as_posix('') empty"

p1 = path_t("/")
p3 = p1%as_posix()
if(p3%path() /= "/") error stop "as_posix '/' failed"

p2 = path_t(char(92))
p3 = p2%as_posix()
if(p3%path() /= "/") error stop "as_posix char(92) failed"

end subroutine test_filesep


subroutine test_filename()

type(path_t) :: p1, p2

if(file_name("") /= "") error stop "filename empty: " // file_name("")

p1 = path_t("a/b/c")
p2 = path_t("a")
if (p1%file_name() /= "c") error stop "file_name failed"
if (p2%file_name() /= "a") error stop "file_name idempotent failed"

if(file_name("file_name") /= "file_name") error stop "file_name leading dot filename"
if(file_name("./file_name") /= "file_name") error stop "file_name leading dot filename cwd"
if(file_name("file_name.txt") /= "file_name.txt") error stop "file_name leading dot filename w/ext"
if(file_name("./file_name.txt") /= "file_name.txt") error stop "file_name leading dot filename w/ext and cwd"
if(file_name("../file_name.txt") /= "file_name.txt") error stop "file_name leading dot filename w/ext up"

end subroutine test_filename


subroutine test_suffix()

type(path_t) :: p1, p2

if(suffix("") /= "") error stop "suffix empty"

p1 = path_t("suffix_name.a.b")

if (p1%suffix() /= ".b") error stop "suffix failed"
p2 = path_t(p1%suffix())
if (p2%suffix() /= "") error stop "suffix nest failed on " // p2%path()
p2 = path_t(p2%suffix())
if (p2%suffix() /= "") error stop "suffix idempotent failed"

if(len_trim(suffix(".suffix")) /= 0) error stop "suffix leading dot filename: " // suffix(".suffix")
if(len_trim(suffix("./.suffix")) /= 0) error stop "suffix leading dot filename cwd: " // suffix("./.suffix")
if(suffix(".suffix.txt") /= ".txt") error stop "suffix leading dot filename w/ext"
if(suffix("./.suffix.txt") /= ".txt") error stop "suffix leading dot filename w/ext and cwd"
if(suffix("../.suffix.txt") /= ".txt") error stop "suffix leading dot filename w/ext up"

end subroutine test_suffix


subroutine test_stem()

type(path_t) :: p1, p2

if(stem("") /= "") error stop "stem empty: " // stem("")

p1 = path_t("stem.a.b")
if (p1%stem() /= "stem.a") error stop "stem failed"
p2 = path_t(p1%stem())
if (p2%stem() /= "stem") error stop "stem nest failed"

if (stem("stem") /= "stem") error stop "stem idempotent failed: " // stem("stem")

if(stem(".stem") /= ".stem") error stop "stem leading dot filename idempotent: " // stem(".stem")
if(stem("./.stem") /= ".stem") error stop "stem leading dot filename cwd"
if(stem(".stem.txt") /= ".stem") error stop "stem leading dot filename w/ext"
if(stem("./.stem.txt") /= ".stem") error stop "stem leading dot filename w/ext and cwd"
if(stem("../.stem.txt") /= ".stem") error stop "stem leading dot filename w/ext up"

end subroutine test_stem


subroutine test_parent()

type(path_t) :: p1, p2


if(parent("") /= ".") error stop "parent empty: " // parent("")

p1 = path_t("a/b/c")
if (p1%parent() /= "a/b") error stop "parent failed" // p1%path()
p2 = path_t(p1%parent())
if (p2%parent() /= "a") error stop "parent nest failed" // p1%path()
p2 = path_t("a")
if (p2%parent() /= ".") error stop "parent idempotent failed. Expected '.', but got: " // p2%path()

if(parent("./.parent") /= ".") error stop "parent leading dot filename cwd"
if(parent(".parent.txt") /= ".") error stop "parent leading dot filename w/ext"
if(parent("./.parent.txt") /= ".") error stop "parent leading dot filename w/ext and cwd"
if(parent("../../.parent.txt") /= "../..") error stop "parent leading dot filename w/ext up"

end subroutine test_parent


subroutine test_with_suffix()

type(path_t) :: p1, p2

if(with_suffix("", ".h5") /= "") error stop "with_suffix empty: " // with_suffix("", ".h5")
if(with_suffix("foo.h5", "") /= "foo") error stop "with_suffix foo.h5 to ''"
if(with_suffix(".h5", "") /= ".h5") error stop "with_suffix .h5 to .h5"
if(with_suffix(".h5", ".h5") /= ".h5.h5") error stop "with_suffix .h5.h5"

p1 = path_t("my/file.h5")
p2 = p1%with_suffix(".hdf5")

if (p2%path() /= "my/file.hdf5") error stop "%with_suffix failed: " // p2%path()
if (p2%path() /= with_suffix("my/file.h5", ".hdf5")) error stop "with_suffix() failed: " // p2%path()

end subroutine test_with_suffix


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
  if(r /= "") error stop "unix %root empty: " // r

  r = root("/etc")
  if(r /= "/") error stop "unix root() failed: " // r
else
  if(p1%root() == "/") error stop "windows %root failed"

  r = p2%root()
  if( r/= "c:") error stop "windows %root drive: " // r
  if(root("c:/etc") /= "c:") error stop "windows root() failed"
endif

end subroutine test_root


subroutine test_is_dir()

character(:), allocatable :: r

integer :: i

type(path_t) :: p1,p2,p3
character(:), allocatable :: iwa

if(is_dir("")) error stop "is_dir empty should be false"

if(sys_posix()) then
  if(.not. is_dir("/")) error stop "is_dir('/') failed"
else
  r = root(get_cwd())
  ! print *, "test_is_dir: root drive: ", r
  if(.not. is_dir(r)) error stop "is_dir('" // r // "') failed"
endif

p1 = path_t(".")

if(.not. p1%is_dir()) error stop "did not detect '.' as directory"
if(p1%is_file()) error stop "detected '.' as file"
call assert_is_dir(".")

iwa = 'test-filesystem.h5'
p2 = path_t(iwa)
open(newunit=i, file=iwa, status='replace')
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
