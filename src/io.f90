submodule (filesystem) io_filesystem
!! procedures that read/write file data

implicit none

contains


module procedure write_text

integer :: u
character(:), allocatable :: iwa

iwa = expanduser(filename)

open(newunit=u, file=iwa, status='unknown', action='write')
write(u,'(A)') text
close(u)

end procedure write_text


module procedure read_text

integer :: L,u
character(:), allocatable :: iwa

L = 16384
if(present(max_length)) L = max_length

iwa = expanduser(filename)

allocate(character(L) :: read_text)

open(newunit=u, file=iwa, status='old', action='read')
read(u,'(A)') read_text
close(u)

end procedure read_text

end submodule io_filesystem
