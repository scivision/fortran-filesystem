program demo

use, intrinsic :: iso_c_binding, only : c_null_char
use pathlib, only : canonical

implicit none (type, external)

character(:), allocatable :: canon_dir, parent_dir, canon_file
character(*), parameter :: dummy = "nobody.txt"

integer :: L1, L2, L3

! -- current directory
canon_dir = canonical(".")
L1 = len_trim(canon_dir)
if (len(canon_dir) < 3) error stop "ERROR canonical '.' " // canon_dir

print *, "OK: current dir = ", canon_dir
! -- relative dir
parent_dir = canonical('..')

L2 = len_trim(parent_dir)
if (L2 >= L1+2) error stop 'ERROR: directory was not canonicalized: ' // parent_dir

print *, 'OK: canon_dir = ', parent_dir
! -- relative file
canon_file = canonical('../' // dummy)
L3 = len_trim(canon_file)
if (L3 - L2 /= len(dummy) + 1) error stop 'ERROR: file was not canonicalized: ' // canon_file

print *, 'OK: canon_file = ', canon_file


print *, "OK: canonical"
end program
