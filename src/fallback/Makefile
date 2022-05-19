FC=gfortran

ifeq ($(OS),Windows_NT)
RM=del
FFLAGS+=-D_WIN32
else
RM=rm
endif


iter.o: ../iter.f90
	$(FC) $(FFLAGS) -c $< -o $@

OBJS=filesystem.o general.o iter.o

.PHONY: all

all: libfilesystem.a

test: test_filesystem

libfilesystem.a: $(OBJS)
	ar rcs $@ $^

test_filesystem: ../../tests/test_filesystem.f90 libfilesystem.a
	$(FC) $(FFLAGS) -o $@ $^

%.o: %.f90
	$(FC) $(FFLAGS) -c $<

%.o: %.F90
	$(FC) -cpp $(FFLAGS) -c $<

clean:
	$(RM) libfilesystem.a *.o *.mod *.smod test_filesystem
