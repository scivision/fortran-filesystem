submodule (filesystem) no_cpp_fs
!! all compilers without C++ filesystem

implicit none

contains


module procedure with_suffix
allocate(character(get_max_path()) :: with_suffix)

if(len_trim(path) > 0) then
  with_suffix = path(:len_trim(path) - len(suffix(path))) // new
else
  with_suffix = ""
endif
end procedure with_suffix


module procedure relative_to

character(:), dimension(:), allocatable :: p1_pts, p2_pts
character(:), allocatable :: s1, s2
integer :: i, N1, N2

allocate(character(get_max_path()) :: relative_to)

s1 = normal(a)
s2 = normal(b)

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
