# Concepts: syscall

Demonstrate different ways of calling processes from a C / C++ program.

We give exec() examples for Windows and Unix-like systems.
To use pipes consider [popen()](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/popen-wpopen).

Notice that Windows CreateProcess() is more complicated to use for simple cases, but does give more control over the process.
