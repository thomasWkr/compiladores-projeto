#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "table.h"

value_t *create_lexic_value(nature_t nature, char *lexeme, int line_number)
{
    value_t *aux = NULL;
    aux = (value_t *)malloc(sizeof(value_t));
    aux->lexeme = strdup(lexeme);
    aux->nature = nature;
    aux->line_number = line_number;
    return aux;
}

content_t *create_content(type_t type, nature_t nature, parameters_t *args, value_t *data)
{
    content_t *aux = NULL;
    aux = (content_t *)malloc(sizeof(content_t));
    aux->type = type;
    aux->nature = nature;
    aux->args = args;
    aux->data = data;
    return aux;
}

symbol_t *new_table(content_t *content, char *key)
{
    symbol_t *sym = malloc(sizeof(symbol_t));
    sym->key = key;
    sym->content = content;
    sym->next_symbol = NULL;
    return sym;
}

void add_symbol(content_t *content, symbol_t *symbol_table, char *key)
{
    symbol_t *sym = malloc(sizeof(symbol_t));
    sym->key = key;
    sym->content = content;
    sym->next_symbol = symbol_table;
    symbol_table = sym;
}

symbol_t *get_symbol(symbol_t *symbol_table, char *key)
{
    for (symbol_t *sym = symbol_table; sym != NULL; sym = sym->next_symbol)
    {
        if (strcmp(sym->key, key) == 0)
        {
            return sym;
        }
    }
    return NULL;
}

void free_table(symbol_t *symbol_table)
{
    symbol_t *sym = symbol_table;
    while (sym != NULL)
    {
        symbol_t *next = sym->next_symbol;

        if (sym->content)
        {
            if (sym->content->args)
            {
                free(sym->content->args);
            }

            if (sym->content->data)
            {
                // free(sym->content->data->lexeme);
                // free(sym->content->data);
            }
            // free(sym->key);
            free(sym->content);
        }
        free(sym);
        sym = next;
    }
}

const char *type_to_str(type_t type)
{
    switch (type)
    {
    case INT:
        return "INT";
    case FLOAT:
        return "FLOAT";
    default:
        return "UNKNOWN_TYPE";
    }
}

const char *nature_to_str(nature_t nature)
{
    switch (nature)
    {
    case LITERAL:
        return "LITERAL";
    case ID:
        return "ID";
    case FUNCTION:
        return "FUNCTION";
    default:
        return "UNKNOWN_NATURE";
    }
}