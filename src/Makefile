.SUFFIXES:
# deactivates implicit file extension so Fortran .mod isn't mistaken for Modula source

CXXFLAGS = -std=c++17
LDFLAGS = -lstdc++

all: pathlib.a

FSRC=pathlib.f90 find.f90 io.f90 iter.f90 fs_cpp.f90
CXXSRC=fs.cpp

FOBJS = $(FSRC:.f90=.o)

fs.o: $(CXXSRC)
	$(CXX) $(CXXFLAGS) -c -o $@ $^

pathlib.a: $(FOBJS) fs.o
	$(AR) cr $@ *.o

%.o %.mod %.smod: %.f90
	$(FC) $(FFLAGS) -c -o $*.o $< $(LDFLAGS)

ifeq ($(OS),Windows_NT)
    RM = del
else
    RM = rm
endif

.PHONY: clean
clean:
	$(RM) pathlib.a *.o *.mod *.smod
