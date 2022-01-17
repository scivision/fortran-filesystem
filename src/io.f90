submodule (pathlib) io_pathlib
!! procedures that read/write file data

implicit none (type, external)

contains

module procedure pathlib_touch
call touch(self%path_str)
end procedure pathlib_touch


module procedure pathlib_write_text
call write_text(self%path_str, text)
end procedure pathlib_write_text


module procedure write_text

integer :: u

open(newunit=u, file=expanduser(filename), status='unknown', action='write')
write(u,'(A)') text
close(u)

end procedure write_text


module procedure pathlib_read_text
pathlib_read_text = read_text(self%path_str, max_length)
end procedure pathlib_read_text


module procedure read_text

integer :: L

L = 16384
if(present(max_length)) L = max_length

block
integer :: u
character(L) :: buf

open(newunit=u, file=expanduser(filename), status='old', action='read')
read(u,'(A)') buf
close(u)

read_text = trim(buf)
end block

end procedure read_text

end submodule io_pathlib
