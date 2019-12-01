
WATCHDOG = watchdog.out
USER = user.out

LIB = libwatchdog.so
LIB_PATH = /home/student/git/workingon

SRC =  *.c
OBJ = $(SRC:.c=.o)

SRC_WD = watchdog.c
SRC_U = user.c

HED_DIR = include

CC = gcc
CFLAGS = -fPIC -c
LFLAGS = -shared
DLFLAGS = -lw -lpthread

.SECONDARY: $(wildcard *.o)
.SUFFIXES:
.PHONY: all clean re

all: $(WATCHDOG) $(USER) $(LIB)

%.o:
	$(CC) $(CFLAGS) $(@:.o=.c) -I $(HED_DIR)

$(LIB): $(OBJ)
	$(CC) $(LFLAGS) $^ $(DLFLAGS) -o $@

$(USER): $(SRC_U)
	$(CC) $< $(DLFLAGS) -lwatchdog -o $@ -I $(HED_DIR)

$(WATCHDOG): $(SRC_WD)
	$(CC) $< $(DLFLAGS) -lwatchdog -o $@ -I $(HED_DIR)

clean:
	rm $(WATCHDOG)
	rm $(USER)
	rm $(LIB)
	rm $(OBJ)

re: clean all
