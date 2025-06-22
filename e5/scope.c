#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "scope.h"
#include "errors.h"

scope *scope_stack = NULL;
args_stack *top_args_stack = NULL;

void create_scope(int scope_type)
{
    scope *new_scope = malloc(sizeof(scope));
    new_scope->symbol_table = NULL;
    switch (scope_type)
    {
    case GLOBAL:
        new_scope->current_offset = 0;
        break;
    case FUNCTION_BLOCK:
        new_scope->current_offset = 0;
        break;
    case COMMAND_BLOCK:
        if (scope_stack != NULL)
            new_scope->current_offset = scope_stack->current_offset;
        else
            new_scope->current_offset = 0;
        break;
    }
    new_scope->type = scope_type;
    new_scope->next_scope = scope_stack;
    scope_stack = new_scope;
}

void update_table(content_t *content, char *key, int symbol_size)
{
    // Table not initialized
    if (!scope_stack->symbol_table)
    {
        if (content->nature == ID)
        {
            scope_stack->symbol_table = new_table(content, key, scope_stack->current_offset);
            scope_stack->current_offset += symbol_size;
        }
        else
        {
            scope_stack->symbol_table = new_table(content, key, -1);
        }
    }
    else
    {
        symbol_t *symbol = get_symbol(scope_stack->symbol_table, key);
        if (symbol)
        {
            // If symbol exists -> ERR_DECLARED
            if (content->nature == ID || content->nature == FUNCTION)
            {
                printf("Row: %d, '%s' already declared on line %d\n", content->data->line_number, key, symbol->content->data->line_number);
                destroy_scope();
                exit(ERR_DECLARED);
            }
            // If symbol exists but is a Literal -> free(entry)
            else
            {
                free(content->data->lexeme);
                free(content->data);
                free(content);
                free(key);
            }
        }
        // If symbol not in Table -> add it
        else
        {
            if (content->nature == ID)
            {
                scope_stack->symbol_table = add_symbol(content, scope_stack->symbol_table, key, scope_stack->current_offset);
                scope_stack->current_offset += symbol_size;
            }
            else
            {
                scope_stack->symbol_table = add_symbol(content, scope_stack->symbol_table, key, -1);
            }
        }
    }
}

void destroy_scope()
{
    if (!scope_stack)
        return;

    if (scope_stack->type == COMMAND_BLOCK)
    {
        update_current_offset();
    }

    // Discomment next line for debugging
    print_scopes();

    scope *top = scope_stack;
    scope_stack = scope_stack->next_scope;
    free_table(top->symbol_table);
    free(top);
}

void update_current_offset()
{
    scope_stack->next_scope->current_offset = scope_stack->current_offset;
}

void check_declared(value_t *data, char *key)
{
    symbol_t *symbol = get_symbol_from_stack(key);
    if (symbol)
    {
        // Using ID as a function call
        if (symbol->content->nature == ID && data->nature == FUNCTION)
        {
            printf("Row: %d, '%s' declared as var on line %d and used as function\n", data->line_number, key, symbol->content->data->line_number);
            destroy_scope();
            exit(ERR_VARIABLE);
        }
        // Using function as a var
        else if (symbol->content->nature == FUNCTION && data->nature == ID)
        {
            printf("Row: %d, '%s' declared as function on line %d and used as var\n", data->line_number, key, symbol->content->data->line_number);
            destroy_scope();
            exit(ERR_FUNCTION);
        }
    }
    else
    {
        printf("Row: %d, '%s' not declared\n", data->line_number, key);
        destroy_scope();
        exit(ERR_UNDECLARED);
    }
}

void create_args_list()
{
    args_stack *new_args_list = malloc(sizeof(args_stack));
    new_args_list->args_list = NULL;
    new_args_list->next_args_list = top_args_stack;
    top_args_stack = new_args_list;
}

void update_args_list(type_t arg_type)
{
    top_args_stack->args_list = add_arg(top_args_stack->args_list, arg_type);
}

void destroy_args_list()
{
    if (!top_args_stack)
        return;

    args_stack *top = top_args_stack;
    top_args_stack = top_args_stack->next_args_list;
    free_args(top->args_list);
    free(top);
}

void compare_args(parameters_t *args_function, value_t *data)
{
    if (args_function)
    {
        if (!top_args_stack->args_list || top_args_stack->args_list->params_count < args_function->params_count)
        {
            printf("Row: %d, Arguments are missing on function call '%s'\n", data->line_number, data->lexeme);
            destroy_args_list();
            destroy_scope();
            exit(ERR_MISSING_ARGS);
        }
        else if (top_args_stack->args_list->params_count > args_function->params_count)
        {
            printf("Row: %d, Too many arguments on function call '%s'\n", data->line_number, data->lexeme);
            destroy_args_list();
            destroy_scope();
            exit(ERR_EXCESS_ARGS);
        }

        for (int i = 0; i < args_function->params_count; i++)
        {
            if (args_function->args_type[i] != top_args_stack->args_list->args_type[i])
            {
                printf("Row: %d, Mismatch of arguments type on function call '%s'\n", data->line_number, data->lexeme);
                destroy_args_list();
                destroy_scope();
                exit(ERR_WRONG_TYPE_ARGS);
            }
        }
    }
    else if (top_args_stack->args_list)
    {
        printf("Row: %d, Too many arguments on function call '%s'\n", data->line_number, data->lexeme);
        destroy_args_list();
        destroy_scope();
        exit(ERR_EXCESS_ARGS);
    }
}

void compare_type(type_t type1, type_t type2, int line_number)
{
    if (type1 != type2)
    {
        printf("Row: %d, type error between '%s' and '%s'\n", line_number, type_to_str(type1), type_to_str(type2));
        destroy_scope();
        exit(ERR_WRONG_TYPE);
    }
}

symbol_t *get_symbol_from_stack(char *key)
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

scope_type get_scope_type(char *key)
{
    for (scope *current_scope = scope_stack; current_scope != NULL; current_scope = current_scope->next_scope)
    {
        symbol_t *symbol = get_symbol(current_scope->symbol_table, key);
        if (symbol)
        {
            return current_scope->type;
        }
    }
    return NULL;
}

symbol_t *get_latest_function()
{
    scope *function_scope = scope_stack;
    while (function_scope->next_scope) // Searches for bottom scope
    {
        function_scope = function_scope->next_scope;
    }
    return function_scope->symbol_table;
}

void print_scopes()
{
    scope *scopes = scope_stack;
    printf("\n======================================================== \n\n");
    while (scopes)
    {
        symbol_t *symbol_table = scopes->symbol_table;
        printf("===== Symbol Table =====\n");
        while (symbol_table != NULL)
        {
            content_t *content = symbol_table->content;
            printf("Key: %s\n", symbol_table->key);
            printf("Offset: %d\n", symbol_table->offset);

            if (content)
            {
                printf("  Type: %s\n", type_to_str(content->type));
                printf("  Nature: %s\n", nature_to_str(content->nature));

                if (content->nature == FUNCTION && content->args)
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
        scopes = scopes->next_scope;
    }
}