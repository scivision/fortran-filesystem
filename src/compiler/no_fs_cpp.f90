submodule (pathlib) no_cpp_fs
!! stub for non-C++17 filesystem

implicit none (type, external)

contains

module procedure exists
error stop "pathlib: exists() requires C++17 filesystem"
end procedure exists

module procedure is_symlink
error stop "pathlib: is_symlink() requires C++17 filesystem"
end procedure is_symlink

module procedure create_symlink
error stop "pathlib: create_symlink() requires C++17 filesystem"
end procedure create_symlink


module procedure same_file
same_file = resolve(path1) == resolve(path2)
end procedure same_file


module procedure relative_to

character(:), dimension(:), allocatable :: p1_pts, p2_pts
character(:), allocatable :: s1, s2
integer :: i, N1, N2

s1 = as_posix(a)
s2 = as_posix(b)

if(s1 == s2) then
!! same path
  relative_to = "."
  return
endif

call file_parts(s1, fparts=p1_pts)
call file_parts(s2, fparts=p2_pts)

N1 = size(p1_pts)
N2 = size(p2_pts)

if(N2 == 0 .or. N1 == 0) then
!! empty
  relative_to = ""
  return
endif

if (N1 < N2+1) then
!! not a subdir of other
  relative_to = ""
  return
endif

if((p1_pts(1) == "/" .and. p2_pts(1) /= "/") .or. (p2_pts(1) == "/" .and. p1_pts(1) /= "/")) then
!! one absolute, one relative
  relative_to = ""
  return
endif

do i = 2, N2
  if(p1_pts(i) /= p2_pts(i)) then
    relative_to = ""
    return
  endif
end do

relative_to = trim(p1_pts(N2+1))
do i = N2+2, N1
  relative_to = join(relative_to, trim(p1_pts(i)))
end do

end procedure relative_to


end submodule no_cpp_fs
