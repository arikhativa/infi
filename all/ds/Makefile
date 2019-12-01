
SRC_DIR = src
LIB_DIR = lib
TEST_DIR = test
HEADER_DIR = include
LIB_PATH =/home/student/git/ds/lib

SRCS := $(wildcard $(SRC_DIR)/*.c)
TEST := $(TEST_DIR)/$(basename %)_test.c
TEST_PATH = $(TEST_DIR)/$(@:.out=_test.c)
TEST_D_PATH = $(TEST_DIR)/$(@:_gdb.out=_test.c)

OBJ := $(SRCS:.c=.o)
OBJ_D := $(SRCS:.c=_gdb.o)

LIB := $(addprefix $(LIB_DIR)/,libds.so)
LIB_D := $(addprefix $(LIB_DIR)/,libds_gdb.so)

EXE := $(notdir $(SRCS:.c=.out))
EXE_D := $(notdir $(SRCS:.c=_gdb.out))

DEP := $(SRCS:.c=.d) $(SRCS:.c=_gdb.d)

CC = gcc
SOFLAGS = -shared
CFLAGS := -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 -fpic
debug: CFLAGS := -ansi -pedantic-errors -Wall -Wextra -g -fpic

.SECONDARY: $(wildcard *.o)
.SUFFIXES:

.PHONY: all release debug test clean re
all: release debug

release: $(DEP) $(LIB) 

debug: $(DEP) $(LIB_D)

test: $(DEP) $(EXE_D) 


%.d: %.c
	$(CC) $(CFLAGS) $(@:.d=.c) -MM -MT $(@:.d=.o) -I $(HEADER_DIR) > $@
	$(CC) $(CFLAGS) $(@:.d=.c) -MM -MT $(@:.d=_gdb.o) -I $(HEADER_DIR) > $(@:.d=_gdb.d)

-include $(DEP)

%.o:
	$(CC) $(CFLAGS) -c $(@:.o=.c) -o $@ -I $(HEADER_DIR)

%_gdb.o:
	$(CC) $(CFLAGS) -c $(@:_gdb.o=.c) -o $@ -I $(HEADER_DIR)


$(LIB): $(OBJ)
	$(CC) $(SOFLAGS) $^ -o $@

$(LIB_D): $(OBJ_D)
	$(CC) $(SOFLAGS) $^ -o $@


%.out: $(TEST) | $(LIB)
	$(CC) -L$(LIB_PATH) -Wl,-rpath=$(LIB_PATH) $(TEST_PATH) -lds -o $@ -I $(HEADER_DIR)

%_gdb.out: $(TEST) | $(LIB_D)
	$(CC) -L$(LIB_PATH) -Wl,-rpath=$(LIB_PATH) $(TEST_D_PATH) -lds_gdb -o $@ -I $(HEADER_DIR)


task.out:
task_gdb.out:

clean:
	rm -f $(LIB) $(LIB_D)
	rm -f $(EXE) $(EXE_D)
	rm -f $(DEP)
	rm -f $(OBJ) $(OBJ_D)

re: clean test
