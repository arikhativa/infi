
NAME = libw.so
PATH_NAME = /usr/lib/

SRC = write_print.c
OBJ = write_print.o

DEP = write_print.h color.h

.SECONDARY: $(wildcard *.o)
.SUFFIXES:
.PHONY: all

all:
	gcc -fPIC -c $(SRC)
	gcc -shared $(OBJ) -o $(NAME)
	echo 308351766 | sudo -S mv $(NAME) $(PATH_NAME)
