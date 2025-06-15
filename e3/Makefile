CC = gcc 
CFLAGS = -I. -fsanitize=address -g 
DEPS = parser.tab.c asd.c lex.yy.c main.c

ODIR=obj

etapa3: $(DEPS)
	$(CC) -Werror -o $@ $^ $(CFLAGS) -lfl -lm 
	
lex.yy.c: scanner.l asd.h parser.tab.h
	flex scanner.l

parser.tab.h parser.tab.c: parser.y
	bison -d parser.y

asd: $(DEPS)
	$(CC) -o asd $^ -I.

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ lex.yy.c parser.tab* etapa* asd