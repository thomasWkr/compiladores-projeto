/* Definition Section*/
%{
#include <stdio.h>
#include <string.h>
#include "table.h"
#include "parser.tab.h"
int get_line_number(void);
%}

%option yylineno
%option noyywrap

/* Rule Section*/
%%

as {return TK_PR_AS;}
declare {return TK_PR_DECLARE;}
else {return TK_PR_ELSE;}
float {return TK_PR_FLOAT;}
if {return TK_PR_IF;}
int {return TK_PR_INT;}
is {return TK_PR_IS;}
return {return TK_PR_RETURN;}
returns {return TK_PR_RETURNS;}
while {return TK_PR_WHILE;}
with {return TK_PR_WITH;}
\<= {return TK_OC_LE;}
\>= {return TK_OC_GE;}
== {return TK_OC_EQ;}
!= {return TK_OC_NE;}

[a-z_][a-z0-9_]* { 
	/*identificador*/
	valor_t *aux = NULL;
	aux = (valor_t*) malloc(sizeof(valor_t));
	if (aux == NULL){
		//KABUM
	}
	aux->lexema = strdup(yytext);
	aux->tipo = 1;
	aux->no_linha = get_line_number();
	yylval.valor_lexico = aux;
	return TK_ID;
}

[0-9]+ {
	/*literal int*/
	valor_t *aux = NULL;
	aux = (valor_t*) malloc(sizeof(valor_t));
	if (aux == NULL){
		//KABUM
	}
	aux->lexema = strdup(yytext);
	aux->tipo = 0;
	aux->no_linha = get_line_number();
	yylval.valor_lexico = aux;

	return TK_LI_INT;
}

[0-9]*\.[0-9]+ {
	/*literal float*/
	valor_t *aux = NULL;
	aux = (valor_t*) malloc(sizeof(valor_t));
	if (aux == NULL){
		//KABUM
	}
	aux->lexema = strdup(yytext);
	aux->tipo = 0;
	aux->no_linha = get_line_number();
	yylval.valor_lexico = aux;
	return TK_LI_FLOAT;
}

[\!\/\*\%\+\-\<\>\(\)\=\,\;\&\||\[|\]] {return (int)yytext[0];} /*token especial*/
[\n\t ] 
\/\/.* /*comentarios*/
. {return TK_ER;}

%%

/* Code Section */ 

int get_line_number(void){
 
 return yylineno;
}
