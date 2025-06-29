#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "asd.h"

asd_tree_t *asd_new(const char *label, value_t *lexic_value, type_t type)
{
  asd_tree_t *ret = NULL;
  ret = calloc(1, sizeof(asd_tree_t));
  if (ret != NULL)
  {
    ret->label = strdup(label);
    ret->number_of_children = 0;
    ret->children = NULL;
    ret->lexic_value = lexic_value;
    ret->type = type;

    ret->code = NULL;
    ret->temp = NULL;
  }
  return ret;
}

void asd_add_code(asd_tree_t *tree, char *code, char *temp)
{
  if (tree != NULL && code != NULL)
  {
    if (temp)
    {
      tree->temp = temp;
    }
    append(&tree->code, code);
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p / %p.\n", __FUNCTION__, tree, code);
  }
}

void asd_copy_code(asd_tree_t *tree, CodeNode **code, char **temp)
{
  if (tree != NULL && code != NULL)
  {
    if (temp)
    {
      tree->temp = *temp;
      *temp = NULL;
    }

    concat_lists(&tree->code, code);
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p / %p.\n", __FUNCTION__, tree, code);
  }
}

void asd_free(asd_tree_t *tree)
{
  if (tree != NULL)
  {
    int i;
    for (i = 0; i < tree->number_of_children; i++)
    {
      asd_free(tree->children[i]);
    }
    if (tree->lexic_value != NULL)
    {
      free(tree->lexic_value->lexeme);
      free(tree->lexic_value);
    }
    if (tree->temp != NULL)
    {
      free(tree->temp);
    }
    if (tree->code != NULL)
    {
      free_list(&tree->code);
    }
    free(tree->children);
    free(tree->label);
    free(tree);
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_add_child(asd_tree_t *tree, asd_tree_t *child)
{
  if (tree != NULL && child != NULL)
  {
    tree->number_of_children++;
    tree->children = realloc(tree->children, tree->number_of_children * sizeof(asd_tree_t *));
    tree->children[tree->number_of_children - 1] = child;
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p / %p.\n", __FUNCTION__, tree, child);
  }
}

static void _asd_print(FILE *foutput, asd_tree_t *tree, int profundidade)
{
  int i;
  if (tree != NULL)
  {
    fprintf(foutput, "%d%*s: Nó '%s' tem %d filhos:\n", profundidade, profundidade * 2, "", tree->label, tree->number_of_children);
    for (i = 0; i < tree->number_of_children; i++)
    {
      _asd_print(foutput, tree->children[i], profundidade + 1);
    }
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_print(asd_tree_t *tree)
{
  FILE *foutput = stderr;
  if (tree != NULL)
  {
    _asd_print(foutput, tree, 0);
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

static void _asd_print_graphviz(FILE *foutput, asd_tree_t *tree)
{
  int i;
  if (tree != NULL)
  {
    fprintf(foutput, "  %ld [ label=\"%s\" ];\n", (long)tree, tree->label);
    for (i = 0; i < tree->number_of_children; i++)
    {
      fprintf(foutput, "  %ld -> %ld;\n", (long)tree, (long)tree->children[i]);
      _asd_print_graphviz(foutput, tree->children[i]);
    }
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_print_graphviz(asd_tree_t *tree)
{
  FILE *foutput = stdout;
  if (tree != NULL)
  {
    fprintf(foutput, "digraph grafo {\n");
    _asd_print_graphviz(foutput, tree);
    fprintf(foutput, "}\n");
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

static void _asd_print_graphviz2(FILE *foutput, asd_tree_t *tree)
{
  int i;
  if (tree != NULL)
  {
    if (tree->lexic_value != NULL)
    {
      fprintf(foutput, "  %ld [ label=\"%s\nlinha %d, natureza %d, tipo %d\" ];\n", (long)tree, tree->label, tree->lexic_value->line_number, tree->lexic_value->nature, tree->type);
    }
    else
    {
      fprintf(foutput, "  %ld [ label=\"%s\" ];\n", (long)tree, tree->label);
    }
    for (i = 0; i < tree->number_of_children; i++)
    {
      fprintf(foutput, "  %ld -> %ld;\n", (long)tree, (long)tree->children[i]);
      _asd_print_graphviz2(foutput, tree->children[i]);
    }
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_print_graphviz2(asd_tree_t *tree)
{
  FILE *foutput = stdout;
  if (tree != NULL)
  {
    fprintf(foutput, "digraph grafo {\n");
    _asd_print_graphviz2(foutput, tree);
    fprintf(foutput, "}\n");
  }
  else
  {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}
