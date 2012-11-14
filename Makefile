VERSION = 1.1
CC      = /usr/bin/gcc
CFLAGS  = -Wall -std=c99 -g -D_REENTRANT -DVERSION=\"$(VERSION)\"
LDFLAGS = -lm 

BIN = iclc
OBJ = snippets.o eval.o store.o ast.o parser.o lexer.o main.o

.PHONY: clean

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

clean:
	rm $(OBJ) $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
