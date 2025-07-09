CC = gcc 
CFLAGS = -I. -fsanitize=address -g 
DEPS = parser.tab.c asd.c table.c scope.c lex.yy.c main.c codelist.c ilochelper.c assembler_helper.c
INCLUDE = asd.h table.h scope.h codelist.h ilochelper.h assembler_helper.h

ODIR=obj

etapa6: $(DEPS)
	$(CC) -Werror -o $@ $^ $(CFLAGS) -lfl -lm 
	
lex.yy.c: scanner.l parser.tab.h $(INCLUDE)
	flex scanner.l

parser.tab.h parser.tab.c: parser.y $(INCLUDE)
	bison -d parser.y

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ lex.yy.c parser.tab* etapa* *.dot *.png