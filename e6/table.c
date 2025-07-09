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

symbol_t *new_table(content_t *content, char *key, int offset, scope_type scope_type)
{
    symbol_t *sym = malloc(sizeof(symbol_t));
    sym->key = key;
    sym->content = content;
    sym->offset = offset;
    sym->scope_type = scope_type;

    sym->next_symbol = NULL;
    return sym;
}

symbol_t *add_symbol(content_t *content, symbol_t *symbol_table, char *key, int offset, scope_type scope_type)
{
    symbol_t *sym = malloc(sizeof(symbol_t));
    sym->key = key;
    sym->content = content;
    sym->offset = offset;
    sym->scope_type = scope_type;

    sym->next_symbol = symbol_table;
    symbol_table = sym;
    return symbol_table;
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
                if (sym->content->args->args_type)
                {
                    free(sym->content->args->args_type);
                }
                free(sym->content->args);
            }

            if (sym->content->data)
            {
                free(sym->content->data->lexeme);
                free(sym->content->data);
            }
            free(sym->key);
            free(sym->content);
        }
        free(sym);
        sym = next;
    }
}

parameters_t *add_arg(parameters_t *params_function, type_t arg)
{
    parameters_t *params = params_function;
    if (!params)
    {
        params = (parameters_t *)malloc(sizeof(parameters_t));
        params->params_count = 0;
        params->args_type = NULL;
    }

    params->args_type = realloc(params->args_type, (params->params_count + 1) * sizeof(type_t));

    params->args_type[params->params_count] = arg;
    params->params_count++;
    return params;
}

void free_args(parameters_t *args)
{
    if (args)
    {
        if (args->args_type)
        {
            free(args->args_type);
        }
        free(args);
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