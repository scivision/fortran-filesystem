program test_rw

use pathlib, only : path_t, read_text, write_text

implicit none (type, external)

character(4096) :: filename
character(:), allocatable :: text, rtext
integer :: L, i
type(path_t) :: p1

if (command_argument_count() /= 1) error stop "please input test filename"
call get_command_argument(1, filename, status=i)
if (i/=0) error stop "please input test filename"

text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit," // &
" sed do eiusmod tempor incididunt ut labore et dolore magna aliqua." // &
" Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat." // &
" Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. " // &
"Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."

L = len(text)

call write_text(filename, text)

rtext = read_text(filename)

if (rtext /= text) error stop "read_text() is not equal to write text"

p1 = path_t(filename)
if (p1%read_text() /= text) error stop "%read_text is not equal to write text"
end program
