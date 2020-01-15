

SRC_DIR = src

SRC = $(wildcard *.c)
EXE = $(SRC:.c=.out)

CC = gcc

.SECONDARY: $(wildcard *.o)
.SUFFIXES:
.PHONY: all clean re

all: $(EXE)

%.out: %.c
	$(CC) $(@:.out=.c) -o $@

clean:
	rm *.out

re: clean all
