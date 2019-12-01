
WATCHDOG = watchdog.out
USER = user.out
LIB = libwatchdog.so

LIB_PATH = /home/student/git/workingon
SRC_DIR = src

SRC = $(SRC_DIR)/*.c
OBJ = $(SRC:.c=.o)

SRC_WD = watchdog.c
SRC_U = user.c

HED_DIR = include

CC = gcc
CFLAGS = -fPIC -c -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3
LFLAGS = -shared
DLFLAGS = -lw -lpthread

.SECONDARY: $(wildcard *.o)
.SUFFIXES:
.PHONY: all clean re

all: $(WATCHDOG) $(USER)

%.o:
	$(CC) $(CFLAGS) $(@:.o=.c) -I $(HED_DIR)
	mv *.o $(SRC_DIR)

%.out: $(LIB)
	$(CC) -L./ -Wl,-rpath=./ $(@:.out=.c) $(DLFLAGS) -lwatchdog -o $@ -I $(HED_DIR)


$(LIB): $(OBJ)
	$(CC) $(LFLAGS) $^ $(DLFLAGS) -o $@

clean:
	rm *.so
	rm *.out
	rm $(OBJ)

re: clean all
