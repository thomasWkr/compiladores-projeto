/* Definition Section*/
%{
#include <stdio.h>
#include <string.h>
#include "asd.h"
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
	yylval.valor_lexico = create_lexic_value(ID, yytext, get_line_number());
	return TK_ID;
}

[0-9]+ {
	/*literal int*/
	yylval.valor_lexico = create_lexic_value(LITERAL, yytext, get_line_number());
	return TK_LI_INT;
}

[0-9]*\.[0-9]+ {
	/*literal float*/
	yylval.valor_lexico = create_lexic_value(LITERAL, yytext, get_line_number());
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