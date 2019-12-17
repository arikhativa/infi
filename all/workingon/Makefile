
NAME = test.out
LIB = ./lib/libpoint.a

LIB_PATH = /home/student/git/workingon/lib
SRC_DIR = src

SRC = $(SRC_DIR)/*.cpp
OBJ = $(SRC:.cpp=.o)

HED_DIR = include

CC = g++
CFLAGS = -c -std=c++98 -pedantic-errors -Wall -Wextra -g
DLFLAGS = -lglut_utils

.SECONDARY: $(wildcard *.o)
.SUFFIXES:
.PHONY: all clean re

all: $(NAME)

%.o: $(SRC)
	$(CC) $(CFLAGS) $(@:.o=.cpp) -I $(HED_DIR)
	mv *.o $(SRC_DIR)

%.out: $(OBJ) $(LIB)
	$(CC) -L$(LIB_PATH) -Wl,-rpath=$(LIB_PATH) $(^) $(DLFLAGS) -o $@ -I $(HED_DIR)

clean:
	rm *.out
	rm src/*.o

re: clean all
