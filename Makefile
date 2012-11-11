VERSION = 3.02
CC      = /usr/bin/gcc
CFLAGS  = -Wall -std=c99 -g -D_REENTRANT -DVERSION=\"$(VERSION)\"
LDFLAGS = -lm 

BIN = iclc
OBJ = ast.o parser.o lexer.o main.o

.PHONY: clean

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

clean:
	rm $(OBJ) $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
