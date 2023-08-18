CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS =

SRC = common.c lexer.c main.c
OBJ = ${SRC:.c=.o}
EXE = cacaml

all: build run clean

build: comp link

comp:
	$(CC) $(CFLAGS) $(SRC) -c

link:
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE) 

debug:
	$(CC) -g $(CFLAGS) $(SRC) -c
	$(CC) -g $(CFLAGS) $(OBJ) -o $(EXE)
	${RM} ${OBJ}

run:
	./$(EXE)

clean:
	${RM} ${OBJ}

cleaner:
	${RM} ${OBJ}
	${RM} ${EXE}
# END
