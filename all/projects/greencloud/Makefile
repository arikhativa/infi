
NAME := greencloud.0.1

SRC_DIR = src
OBJ_DIR = obj
HED_DIR = include
LIB_DIR = lib
RELEASE_DIR = release
TEST_DIR = test

VPATH = $(TEST_DIR):$(OBJ_DIR):$(SRC_DIR):$(RELEASE_DIR)


SRC := $(wildcard $(SRC_DIR)/*.cpp)
EXE := $(NAME).out
NAME_SRC := $(NAME).cpp
OBJ := $(notdir $(SRC:.cpp=.o))
LIB := $(LIB_DIR)/lib.a
OBJS :=$(addprefix $(OBJ_DIR)/,$(OBJ))
TESTS := $(wildcard $(TEST_DIR)/*.cpp)
TESTS_OUT := $(TESTS:.cpp=.out)

CC = g++
CFLAGS = -c -std=c++11 -pedantic-errors -Wall -Wextra -g
LFLAGS = -std=c++11 -pedantic-errors -Wall -Wextra -g -pthread

.SECONDARY: $(wildcard *.o)
.SUFFIXES:
.PHONY: all clean re

all:$(EXE)

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) $^ -I $(HED_DIR)
	mv *.o $(OBJ_DIR)

#$(LIB): $(OBJ)
#	ar rc $@ $(addprefix $(OBJ_DIR)/,$(OBJ))

$(EXE): $(NAME_SRC) $(OBJ)
	$(CC) $(LFLAGS) $< $(OBJS) -o $@ -I $(HED_DIR)

singleton:
	$(CC) $(LFLAGS) ./test/singleton_test.cpp -o $@.out -ldl -I $(HED_DIR)


clean:
	rm *.out
#	rm $(LIB_DIR)/*.a
	rm $(OBJ_DIR)/*.o

re: clean all
