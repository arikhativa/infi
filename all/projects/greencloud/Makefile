
SRC_DIR = /home/student/git/projects/greencloud/src
OBJ_DIR = /home/student/git/projects/greencloud/obj
HED_DIR = /home/student/git/projects/greencloud/include
LIB_DIR = /home/student/git/projects/greencloud/lib
TEST_DIR = /home/student/git/projects/greencloud/test

VPATH = $(TEST_DIR):$(OBJ_DIR):$(SRC_DIR)

SRC := $(wildcard $(SRC_DIR)/*.cpp)
EXE := ver.0.1.out
OBJ := $(notdir $(SRC:.cpp=.o))
LIB := $(LIB_DIR)/lib.a

CC = g++
CFLAGS = -c -std=c++11 -pedantic-errors -Wall -Wextra -g
LFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -g

.SECONDARY: $(wildcard *.o)
.SUFFIXES:
.PHONY: all clean re

all: $(EXE)

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -I $(HED_DIR)
	mv *.o $(OBJ_DIR)

$(LIB): $(OBJ)
	ar rc $@ $(addprefix $(OBJ_DIR)/,$(OBJ))

$(EXE): $(LIB) ver.0.1.cpp
	$(CC) $(LFLAGS) $^ -o $@ -I $(HED_DIR)

clean:
	rm *.out
	rm $(LIB_DIR)/*.a
	rm $(OBJ_DIR)/*.o

re: clean all
