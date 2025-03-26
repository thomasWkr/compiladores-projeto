CC = gcc
CFLAGS = -I.
DEPS = tokens.h

ODIR=obj

_OBJ = lex.yy.o main.o
OBJ = $(patsubst %, $(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)
	
	
etapa1: $(OBJ)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ $(CFLAGS)
	
lex.yy.c: scanner.l
	flex scanner.l
	
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ lex.yy.c
