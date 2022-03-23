program ex1

use filesystem

implicit none (type, external)

print '(a)', "current working dir: " // get_cwd()

print '(a)', "home dir: " // get_homedir()

end program
