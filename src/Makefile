CC = gcc
CRELFLAGS = -std=c99
CDEVFLAGS = -pedantic -Werror -Wall -Wextra -Wvla -Wshadow -g
LDLIBS =

SRC = `find . ../utils -name '*.c'`
OBJ = `find . ../utils -name '*.o'`
SSA = `find . -name '*.ssa'`
ASM = `find . -name '*.asm'`
EXE = cacaml

all: setrel compile

compile:
	$(CC) -o $(EXE) $(CFLAGS) $(SRC)
rel: setrel compile
dev: setdev compile
run:
	./$(EXE)
gdb: dev
	gdb ./$(EXE)
memcheck: dev
	valgrind --leak-check=full -s ./$(EXE)

setdev:
	$(eval CFLAGS := $(CDEVFLAGS))
setrel:
	$(eval CFLAGS := $(CRELFLAGS))

.PHONY: setdev setrel debug mop clean

mop:
	$(RM) $(OBJ)
	$(RM) $(SSA)
	$(RM) $(ASM)

clean: mop
	$(RM) $(EXE)
