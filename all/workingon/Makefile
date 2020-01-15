

SRC_DIR = src
HED_DIR = include

SRC = $(wildcard src/*.cpp) $(wildcard src/*.c)
OUT = $(wildcard *.cpp)
EXE = $(OUT:.cpp=.out)

CC = g++

.SECONDARY: $(wildcard *.o)
.SUFFIXES:
.PHONY: all clean re

all: $(EXE)

%.out: %.cpp $(SRC)
	$(CC) $(@:.out=.cpp) $(SRC) -o $@ -I $(HED_DIR)

clean:
	rm *.out

re: clean all
