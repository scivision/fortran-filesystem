program ex1

use filesystem

implicit none

print '(a)', "current working dir " // get_cwd()

print '(a)', "home dir " // get_homedir()

print '(a)', "expanduser('~') " // expanduser('~')

end program
