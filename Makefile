NAME := fs_cli

INC := -Iinclude/
CXXFLAGS := -std=c++20 -Wall $(INC)
CFLAGS := -Wall $(INC)

cdir = src/common/
SRCS = $(cdir)common.c $(cdir)filesystem.cpp
OBJS_CXX := ${SRCS:.cpp=.o}
OBJS_C := ${SRCS:.c=.o}

all: $(NAME)

$(NAME): app/main.cpp libfilesystem.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -Iinclude/

libfilesystem.a: $(OBJS_CXX) $(OBJS_C)
	ar rcs $@ $^

.PHONY: all

clean:
	$(RM) $(cdir)common.o $(cdir)filesystem.o libfilesystem.a $(NAME)
