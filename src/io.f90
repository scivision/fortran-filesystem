submodule (filesystem) io_filesystem
!! procedures that read/write file data

implicit none (type, external)

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

integer :: L

L = 16384
if(present(max_length)) L = max_length

block
integer :: u
character(L) :: buf
character(:), allocatable :: iwa

iwa = expanduser(filename)

open(newunit=u, file=iwa, status='old', action='read')
read(u,'(A)') buf
close(u)

read_text = trim(buf)
end block

end procedure read_text

end submodule io_filesystem
