/*
  Programa principal de impressão de tokens.
  Este arquivo será posteriormente substituído.
*/
#include <stdio.h>
#define _(s) #s // https://gcc.gnu.org/onlinedocs/gcc-12.2.0/cpp/Stringizing.html
#include "parser.tab.h"

extern int yylex_destroy(void);

extern FILE *yyin;
extern char *yytext;
extern int get_line_number (void);


int main (int argc, char **argv) {
  int ret = yyparse();
  yylex_destroy();
  return ret;
}
