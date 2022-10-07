submodule (filesystem) io_filesystem
!! procedures that read/write file data

implicit none

contains


module procedure write_text
integer :: u

open(newunit=u, file=expanduser(filename), status='unknown', action='write')
write(u,'(A)') text
close(u)

end procedure


module procedure read_text

integer :: L,u

L = 16384
if(present(max_length)) L = max_length

allocate(character(L) :: read_text)

open(newunit=u, file=expanduser(filename), status='old', action='read')
read(u,'(A)') read_text
close(u)

end procedure

end submodule io_filesystem
