program main

use, intrinsic :: iso_fortran_env, only : stderr=>error_unit
use filesystem

implicit none

integer :: i
character(9) :: p

character(:), allocatable :: reada, noread

allocate(character(get_max_path()) :: reada, noread)

if(command_argument_count() < 2) error stop "specify  <readable file> <non-readable file>"

call get_command_argument(1, reada, status=i)
if(i /= 0) error stop "error getting readable file name"

call get_command_argument(2, noread, status=i)
if(i /= 0) error stop "error getting non-readable file name"

p = get_permissions("")
if(len_trim(p) /= 0) then
    write(stderr, '(a)') "get_permissions('') should be empty: " // p
    error stop
endif

p = get_permissions(reada)
print '(a)', "Permissions for " // trim(reada)// ": "// p

if (len_trim(p) == 0) error stop "get_permissions('"//trim(reada)//"') should not be empty"

if (p(1:1) /= "r") then
    write(stderr, '(a)') "ERROR: test_exe: "//trim(reada)//" should be readable"
    error stop
endif

if(.not. exists(reada)) error stop "test_exe: "//trim(reada)//" should exist"

if(.not. is_file(reada)) error stop "test_exe: "//trim(reada)//" should be a file"

!! for Ffilesystem, even non-readable files "exist" and are "is_file"

p = get_permissions(noread);
print '(a)', "Permissions for " // trim(noread)// ": "// p

if (len_trim(p) == 0) error stop "get_permissions('"//trim(noread)//"') should not be empty"

if (index(p, "r") /= 0) then
    write(stderr, '(a)') "test_exe: "//trim(noread)//" should not be readable"
    error stop 77
endif

if (.not. exists(noread)) error stop "test_exe: "//trim(noread)//" should exist"

if (.not. is_file(noread)) error stop "test_exe: "//trim(noread)//" should be a file"

end program
