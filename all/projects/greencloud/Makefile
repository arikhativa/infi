
SRC_DIR = src
OBJ_DIR = obj
HED_DIR = include
LIB_DIR = lib
TEST_DIR = test

VPATH = $(TEST_DIR):$(OBJ_DIR):$(SRC_DIR)

SRC := $(wildcard $(SRC_DIR)/*.cpp)
EXE := ver.0.1.out
OBJ := $(notdir $(SRC:.cpp=.o))
LIB := $(LIB_DIR)/lib.a
OBJS :=$(addprefix $(OBJ_DIR)/,$(OBJ))

CC = g++
CFLAGS = -c -std=c++11 -pedantic-errors -Wall -Wextra -g
LFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -g -lpthread

.SECONDARY: $(wildcard *.o)
.SUFFIXES:
.PHONY: all clean re

all:$(EXE)

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -I $(HED_DIR)
	mv *.o $(OBJ_DIR)

#$(LIB): $(OBJ)
#	ar rc $@ $(addprefix $(OBJ_DIR)/,$(OBJ))

$(EXE): ver.0.1.cpp $(OBJ)
	$(CC) $(LFLAGS) $< $(OBJS) -o $@ -I $(HED_DIR)

clean:
	rm *.out
#	rm $(LIB_DIR)/*.a
	rm $(OBJ_DIR)/*.o

re: clean all
