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
        scope_stack->symbol_table = new_table(content, key);
    }
    else
    {
        symbol_t *symbol = get_symbol(scope_stack->symbol_table, key);
        if (get_symbol(scope_stack->symbol_table, key))
        {
            if (content->nature == ID || content->nature == FUNCTION)
            {
                printf("Row: %d, %s already declared on line %d\n", content->data->line_number, key, symbol->content->data->line_number);
                exit(ERR_DECLARED);
            }
        }
        else
        {
            add_symbol(content, scope_stack->symbol_table, key);
        }
    }
}

void destroy_scope()
{
    if (!scope_stack)
        return;

    // Discomment for debugging
    print_table();

    scope *top = scope_stack;
    scope_stack = scope_stack->next_scope;
    free_table(top->symbol_table);
    free(top);
}

void check_declared(value_t *data, char *key)
{
    symbol_t *symbol = get_symbol_from_scope(key);
    if (symbol)
    {
        if (symbol->content->nature == ID && data->nature == FUNCTION)
        {
            printf("Row: %d, %s declared as var on line %d and used as function\n", data->line_number, key, symbol->content->data->line_number);
            exit(ERR_VARIABLE);
        }
        else if (symbol->content->nature == FUNCTION && data->nature == ID)
        {
            printf("Row: %d, %s declared as function on line %d and used as var\n", data->line_number, key, symbol->content->data->line_number);
            exit(ERR_FUNCTION);
        }
    }
    else
    {
        printf("Row: %d, %s not declared\n", data->line_number, key);
        exit(ERR_UNDECLARED);
    }
}

void compare_type(type_t type1, type_t type2)
{
    if (type1 != type2)
    {
        printf("Row: , type error");
        exit(ERR_WRONG_TYPE);
    }
}

symbol_t *get_symbol_from_scope(char *key)
{
    for (scope *current_scope = scope_stack; current_scope != NULL; current_scope = current_scope->next_scope)
    {
        symbol_t *symbol = get_symbol(current_scope->symbol_table, key);
        if (symbol)
        {
            return symbol;
        }
    }
    return NULL;
}

symbol_t *get_latest_function()
{
    scope *function_scope = scope_stack;
    while (function_scope->next_scope)
    {
        function_scope = function_scope->next_scope;
    }
    return function_scope->symbol_table;
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