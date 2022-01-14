program test_symlink

use pathlib, only : path_t, is_symlink

implicit none (type, external)

integer :: i
type(path_t) :: p_sym, p_nonsym
character(2048) :: buf

character(:), allocatable :: non_symlink_path, symlink_path

if(command_argument_count() /= 2) error stop "usage: test_symlink <non-symlink> <symlink>"

call get_command_argument(1, buf, status=i)
if(i /= 0) error stop "please specify non-symlink path to test"
non_symlink_path = trim(buf)
p_nonsym = path_t(non_symlink_path)

call get_command_argument(2, buf, status=i)
if(i /= 0) error stop "please specify symlink path to test"
symlink_path = trim(buf)
p_sym = path_t(symlink_path)

if(is_symlink("not-exist-path.nobody")) error stop "is_symlink() should be false for non-existant path"

if(is_symlink(non_symlink_path)) error stop "is_symlink() should be false for non-symlink path"
if(p_nonsym%is_symlink()) error stop "%is_symlink() should be false for non-symlink path"

if(.not. is_symlink(symlink_path)) error stop "is_symlink() should be true for symlink path"
if(.not. p_sym%is_symlink()) error stop "%is_symlink() should be trum for symlink path"

end program
