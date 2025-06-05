#include "table.h"
#include "errors.h"

typedef struct scope
{
    symbol_t *symbol_table;
    struct scope *next_scope;
} scope;

typedef struct args_stack
{
    parameters_t *args_list;
    struct args_stack *next_args_list;
} args_stack;

void create_scope();

void update_table(content_t *content, char *key);

void destroy_scope();

void check_declared(value_t *data, char *key);

void create_args_list();

void update_args_list(type_t args_type);

void destroy_args_list();

void compare_args(parameters_t *args_function, value_t *data);

void compare_type(type_t type1, type_t type2);

symbol_t *get_symbol_from_scope(char *key);

symbol_t *get_latest_function();

void print_scopes();