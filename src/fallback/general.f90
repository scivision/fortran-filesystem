submodule (filesystem) no_cpp_fs
!! all compilers without non-C++17 filesystem

implicit none

contains


module procedure as_posix

integer :: i

allocate(character(len(path)) :: as_posix)
as_posix = path

i = index(as_posix, char(92))
do while (i > 0)
  as_posix(i:i) = '/'
  i = index(as_posix, char(92))
end do

as_posix = drop_sep(as_posix)

end procedure as_posix


pure function drop_sep(path)
character(*), intent(in) :: path
character(:), allocatable :: drop_sep

integer :: i

allocate(character(len(path)) :: drop_sep)
drop_sep = path

i = index(drop_sep, "//")
do while (i > 0)
  drop_sep(i:) = drop_sep(i+1:)
  i = index(drop_sep, "//")
end do
end function


module procedure is_file
character(:), allocatable :: iwa

iwa = expanduser(path)
inquire(file=iwa, exist=is_file)
if(is_file) then
  if (is_dir(path)) is_file = .false.
endif
end procedure is_file


module procedure same_file
same_file = canonical(path1) == canonical(path2)
end procedure same_file


module procedure file_name
character(:), allocatable :: wk
integer :: i

wk = as_posix(path)

i = index(wk, "/", back=.true.) + 1

allocate(character(len_trim(wk(i:))) :: file_name)

file_name = wk(i:)

!print *, "TRACE:file_name: in: " // wk // " out: " // file_name

end procedure file_name


module procedure stem
character(len_trim(path)) :: wk
integer :: i

allocate(character(get_max_path()) :: stem)

wk = file_name(path)

i = index(wk, '.', back=.true.)
if (i > 1) then
  stem = wk(:i - 1)
else
  stem = wk
endif
!print '(a,i0,a)', "TRACE:stem: in: " // wk // " i: ", i, " out: " // stem
end procedure stem


module procedure parent
character(:), allocatable :: wk
integer :: i

allocate(character(get_max_path()) :: parent)

wk = as_posix(path)

i = index(wk, "/", back=.true.)
if (i > 0) then
  parent = wk(:i-1)
else
  parent = "."
end if
end procedure parent


module procedure suffix
character(:), allocatable :: wk
integer :: i

allocate(character(get_max_path()) :: suffix)

wk = file_name(path)

i = index(wk, '.', back=.true.)

if (i > 1) then
  suffix = wk(i:)
else
  suffix = ''
end if
!print '(a,i0,a)', "TRACE:suffix: in: " // wk // " i: ", i, " out: " // suffix
end procedure suffix


module procedure with_suffix

allocate(character(get_max_path()) :: with_suffix)

if(len_trim(path) > 0) then
  with_suffix = path(:len_trim(path) - len(suffix(path))) // new
else
  with_suffix = ""
endif

end procedure with_suffix


module procedure touch

integer :: u
character(:), allocatable :: fn

fn = expanduser(path)

if(is_file(fn)) then
  return
elseif(is_dir(fn)) then
  error stop "filesystem:touch: cannot touch directory: " // fn
end if

open(newunit=u, file=fn, status='new')
close(u)

if(.not. is_file(fn)) error stop 'could not touch ' // fn

end procedure touch


module procedure relative_to

character(:), dimension(:), allocatable :: p1_pts, p2_pts
character(:), allocatable :: s1, s2
integer :: i, N1, N2

allocate(character(get_max_path()) :: relative_to)

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


module procedure get_homedir
!! returns home directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/Home_directory#Default_home_directory_per_operating_system

character(:), allocatable :: buf
integer :: istat, L

allocate(character(get_max_path()) :: buf)

if(sys_posix()) then
  call get_environment_variable("HOME", buf, length=L, status=istat)
else
  call get_environment_variable("USERPROFILE", buf, length=L, status=istat)
endif

allocate(character(L) :: get_homedir)

if (istat == 0) then
  get_homedir = as_posix(buf)
else
  get_homedir = ""
endif

end procedure get_homedir


module procedure get_tempdir
!! returns temporary directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/TMPDIR

character(:), allocatable :: buf
integer :: istat, L

allocate(character(get_max_path()) :: buf)

if(sys_posix()) then
  call get_environment_variable("TMPDIR", buf, length=L, status=istat)
else
  call get_environment_variable("TMP", buf, length=L, status=istat)
endif

allocate(character(L) :: get_tempdir)

if (istat == 0) then
  get_tempdir = as_posix(buf)
else
  get_tempdir = ""
endif

end procedure get_tempdir


module procedure expanduser
character(:), allocatable :: home

allocate(character(get_max_path()) :: expanduser)

expanduser = adjustl(path)

if (len_trim(expanduser) == 0) return
if(expanduser(1:1) /= '~') return

home = trim(get_homedir())
if (len_trim(home) == 0) return

if (len_trim(expanduser) < 2) then
  !! ~ alone
  expanduser = home
else
  !! ~/...
  expanduser = home // expanduser(2:)
endif

expanduser = as_posix(expanduser)

end procedure expanduser

end submodule no_cpp_fs
