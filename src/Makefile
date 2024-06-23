CC = gcc
CRELFLAGS = -std=c99
CDEVFLAGS = -pedantic -Werror -Wall -Wextra -Wvla -Wshadow -g
LDLIBS =

SRC = `find . ../utils -name '*.c'`
OBJ = `find . ../utils -name '*.o'`
EXE = cacaml

all: setrel compile

compile:
	$(CC) -o $(EXE) $(CFLAGS) $(SRC)
dev: setdev compile
gdb: debug
	gdb ./$(EXE)

setdev:
	$(eval CFLAGS := $(CDEVFLAGS))
setrel:
	$(eval CFLAGS := $(CRELFLAGS))

.PHONY: setdev setrel debug mop clean

mop:
	$(RM) $(OBJ)

clean: mop
	$(RM) $(EXE)
