program mkd

use filesystem, only : make_tempdir, is_dir

implicit none


block
character(:), allocatable :: temp_dir

temp_dir = make_tempdir()

if(.not. is_dir(temp_dir)) error stop "test_mkdtemp: temp dir not created " // temp_dir

print '(a)', "OK: Fortran mkdtemp: " // temp_dir
end block !< valgrind tweak

end program
