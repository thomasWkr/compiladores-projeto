CC = gcc
CFLAGS = -I.
DEPS = parser.tab.h

ODIR=obj

_OBJ = lex.yy.o main.o parser.tab.o
OBJ = $(patsubst %, $(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)
	
etapa2: $(OBJ)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ $(CFLAGS)
	
lex.yy.c: scanner.l parser.tab.h
	flex scanner.l
	
parser.tab.h parser.tab.c: parser.y
	bison -d parser.y

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ lex.yy.c parser.tab.h parser.tab.c