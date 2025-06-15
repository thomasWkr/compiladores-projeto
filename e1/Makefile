CC = gcc
CFLAGS = -I.
DEPS = tokens.h
OBJ = lex.yy.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
etapa1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	
lex.yy.c: scanner.l
	flex scanner.l
	
.PHONY: clean

clean:
	rm -f *.o *~ lex.yy.c
