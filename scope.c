#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scope.h"
#include "errors.h"

scope *scope_stack = NULL;

void create_scope()
{
    scope *new_scope = malloc(sizeof(scope));
    new_scope->symbol_table = NULL;
    new_scope->next_scope = scope_stack;
    scope_stack = new_scope;
}

void update_table(content_t *content, char *key)
{
    if (!scope_stack->symbol_table)
    {
        scope_stack->symbol_table = new_table(content);
    }
    else
    {
        if (get_symbol(scope_stack->symbol_table, key))
        {
            if (content->nature == ID || content->nature == FUNCTION)
                exit(ERR_DECLARED);
        }
        else
        {
            add_symbol(content, scope_stack->symbol_table, key);
        }
    }
}

void check_declared(content_t *content, char *key)
{
    symbol_t *symbol = get_symbol_from_scope(key);
    if (symbol)
    {
        if (symbol->content->nature == ID && content->nature == FUNCTION)
        {
            exit(ERR_VARIABLE);
        }
        else if (symbol->content->nature == ID && content->nature == FUNCTION)
        {
            exit(ERR_FUNCTION);
        }
    }
    else
    {
        exit(ERR_UNDECLARED);
    }
}

void compare_type(content_t *content1, content_t *content2)
{
    symbol_t *symbol = get_symbol_from_scope(key);
    if (symbol)
    {
        if (symbol->content->nature == ID && content->nature == FUNCTION)
        {
            exit(ERR_VARIABLE);
        }
        else if (symbol->content->nature == ID && content->nature == FUNCTION)
        {
            exit(ERR_FUNCTION);
        }
    }
    else
    {
        exit(ERR_UNDECLARED);
    }
}

void destroy_scope()
{
    if (!scope_stack)
        return;

    // Discomment for debugging
    print_table(scope_stack->symbol_table);

    scope *top = scope_stack;
    scope_stack = scope_stack->next_scope;
    free_table(top->symbol_table);
    free(top);
}

symbol_t *get_symbol_from_scope(char *key)
{
    for (scope *current_scope = scope_stack; current_scope != NULL; current_scope = current_scope->next_scope)
    {
        symbol_t *symbol = get_symbol(current_scope->symbol_table);
        if (symbol)
        {
            return symbol;
        }
    }
    return NULL;
}

void print_table()
{
    symbol_t *symbol_table = scope_stack->symbol_table;
    printf("===== Symbol Table =====\n");
    while (symbol_table != NULL)
    {
        content_t *content = symbol_table->content;
        printf("Key: %s\n", symbol_table->key);

        if (content)
        {
            printf("  Type: %s\n", type_to_str(content->type));
            printf("  Nature: %s\n", nature_to_str(content->data->nature));

            if (content->data->nature == FUNCTION)
            {
                printf("  Parameters (%d): ", content->args->params_count);
                for (int i = 0; i < content->args->params_count; i++)
                {
                    printf("%s ", type_to_str(content->args->args_type[i]));
                }
                printf("\n");
            }

            if (content->data)
            {
                printf("  Lexeme: %s\n", content->data->lexeme);
                printf("  Line: %d\n", content->data->line_number);
                printf("  Data Nature: %s\n", nature_to_str(content->data->nature));
            }
        }

        printf("------------------------\n");
        symbol_table = symbol_table->next_symbol;
    }
}