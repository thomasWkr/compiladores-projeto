alteracao: scanner.l main.c
	flex scanner.l 
	gcc lex.yy.c -c -I.
	gcc main.c -c -I.
	gcc main.o lex.yy.o -o etapa1 