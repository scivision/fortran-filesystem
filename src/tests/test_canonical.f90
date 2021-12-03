program demo

use, intrinsic :: iso_c_binding, only : c_null_char
use pathlib, only : path_t

implicit none (type, external)

type(path_t) :: parent, file, p1, p2
character(*), parameter :: dummy = "nobody.txt"

integer :: L2, L3

! ! -- current directory  -- old MacOS doesn't handle "." or ".." alone
! cwd = path_t(".")
! cwd = cwd%resolve()
! L1 = cwd%length()
! if (L1 < 3) error stop "ERROR canonical '.' " // cwd%path()

! print *, "OK: current dir = ", cwd%path()
! -- home directory
p1 = path_t("~")
p1 = p1%resolve()
if (p1%path(1,1) == "~") error stop "resolve ~" // p1%path()
print *, "OK: home dir = ", p1%path()

p2 = path_t(p1%parent())
if (p2%length() >= p1%length()) error stop "parent home " // p2%path()

! -- relative dir
parent = path_t("~/..")
parent = parent%resolve()

L2 = parent%length()
if (L2 < 1) error stop 'ERROR: directory was not canonicalized: ' // parent%path()

print *, 'OK: parent = ', parent%path()
! -- relative file
file = path_t('~/../' // dummy)
file = file%resolve()
L3 = file%length()
if (L3 - L2 /= len(dummy) + 1) error stop 'ERROR: file was not canonicalized: ' // file%path()

print *, 'OK: canon_file = ', file%path()

! --- same_file
p1 = path_t("a/c")
p2 = path_t("a/b/../c")

if (.not. p1%same_file(p2)) error stop 'ERROR: same_file'


print *, "OK: canonical"
end program
