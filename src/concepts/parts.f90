

integer(C_SIZE_T) function fs_file_parts(path, parts) bind(C, name="file_parts")
import
character(kind=c_char), intent(in) :: path(*)
type(c_ptr), target, allocatable, intent(out) :: parts(:)
end function fs_file_parts

  ! size_t strlen(char * s);
integer(c_size_t) function strlen(s) bind(C, name='strlen')
import
type(c_ptr), intent(in), value :: s
end function strlen
