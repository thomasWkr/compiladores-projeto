#include "table.h"

typedef struct scope
{
    symbol_t *symbol_table;
    struct scope *next_scope;
} scope;

void create_scope();

void update_table(content_t *content, char *key);

void destroy_scope();

void check_declared(value_t *data, char *key);

void compare_type(type_t type1, type_t type2);

symbol_t *get_symbol_from_scope(char *key);

symbol_t *get_latest_function();

void print_table();