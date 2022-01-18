submodule (pathlib) no_cpp_fs
!! stub for non-C++17 filesystem

implicit none (type, external)

contains


module procedure normal
error stop "pathlib: normal() requires C++17 filesystem"
end procedure normal

module procedure exists
error stop "pathlib: exists() requires C++17 filesystem"
end procedure exists

module procedure is_symlink
error stop "pathlib: is_symlink() requires C++17 filesystem"
end procedure is_symlink

module procedure create_symlink
error stop "pathlib: create_symlink() requires C++17 filesystem"
end procedure create_symlink

module procedure get_tempdir
error stop "pathlib: get_tempdir() requires C++17 filesystem"
end procedure get_tempdir


module procedure is_file
inquire(file=expanduser(path), exist=is_file)
if(is_file) then
  if (is_dir(path)) is_file = .false.
endif
end procedure is_file


module procedure same_file
same_file = resolve(path1) == resolve(path2)
end procedure same_file


module procedure file_name
character(:), allocatable :: wk

wk = as_posix(path)
file_name = trim(wk(index(wk, "/", back=.true.) + 1:))
end procedure file_name


module procedure stem
character(len_trim(path)) :: wk
integer :: i

wk = file_name(path)

i = index(wk, '.', back=.true.)
if (i > 0) then
  stem = wk(:i - 1)
else
  stem = wk
endif
end procedure stem


module procedure parent
character(:), allocatable :: wk
integer :: i

wk = as_posix(path)

i = index(wk, "/", back=.true.)
if (i > 0) then
  parent = wk(:i-1)
else
  parent = "."
end if
end procedure parent


module procedure suffix
integer :: i

i = index(path, '.', back=.true.)

if (i > 1) then
  suffix = trim(path(i:))
else
  suffix = ''
end if
end procedure suffix


module procedure with_suffix
with_suffix = path(1:len_trim(path) - len(suffix(path))) // trim(new)
end procedure with_suffix


module procedure touch

integer :: u
character(:), allocatable :: fn

fn = expanduser(path)

if(is_file(fn)) then
  return
elseif(is_dir(fn)) then
  error stop "pathlib:touch: cannot touch directory: " // fn
end if

open(newunit=u, file=fn, status='new')
close(u)

if(.not. is_file(fn)) error stop 'could not touch ' // fn

end procedure touch


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
