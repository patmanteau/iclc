VERSION = 1.1
CC      = /usr/bin/clang
CFLAGS  = -Wall -std=c99 -g -DHAVE_READLINE -D_REENTRANT -DVERSION=\"$(VERSION)\"
LDFLAGS = -lm -lreadline

BIN = iclc
OBJ = generic_list.o snippets.o eval.o store.o ast.o parser.o lexer.o main.o

.PHONY: clean

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) $(LDFLAGS)

clean:
	rm $(OBJ) $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
