#include <stdio.h>
#include "asd.h"
extern int yyparse(void);
extern int yylex_destroy(void);
asd_tree_t *arvore = NULL;
int main(int argc, char **argv)
{
  int ret = yyparse();
  print_list(arvore->code);
  asd_free(arvore);
  yylex_destroy();
  return ret;
}
