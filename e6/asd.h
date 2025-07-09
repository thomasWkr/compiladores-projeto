#ifndef _ASD_H_
#define _ASD_H_
#include "scope.h"
#include "codelist.h"

typedef struct asd_tree
{
  char *label;
  int number_of_children;
  char *temp;
  CodeNode *code;
  struct asd_tree **children;
  value_t *lexic_value;
  type_t type;
} asd_tree_t;

/*
 * Função asd_new, cria um nó sem filhos com o label informado.
 */
asd_tree_t *asd_new(const char *label, value_t *lexic_value, type_t type);

/*
 * Função asd_add_code, adiciona o código e o temporário do nodo.
 */
void asd_add_code(asd_tree_t *tree, char *code, char *temp);

/*
 * Função asd_copy_code, copia o código e o temporário dos nodos abaixo.
 */
void asd_copy_code(asd_tree_t *tree, CodeNode **code, char **temp);

/*
 * Função asd_tree, libera recursivamente o nó e seus filhos.
 */
void asd_free(asd_tree_t *tree);

/*
 * Função asd_add_child, adiciona child como filho de tree.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/*
 * Função asd_print, imprime recursivamente a árvore.
 */
void asd_print(asd_tree_t *tree);

/*
 * Função asd_print_graphviz, idem, em formato DOT
 */
void asd_print_graphviz(asd_tree_t *tree);

/*
 * Função asd_print_graphviz2, idem, em formato DOT
 */
void asd_print_graphviz2(asd_tree_t *tree);
#endif //_ASD_H_
