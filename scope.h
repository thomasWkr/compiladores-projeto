#include "table.h"

typedef struct scope
{
    symbol_t *symbol_table;
    struct scope *next_scope;
} scope;

void create_scope();

void update_table(content_t *content, char *key);

void check_declared(content_t *content, char *key);

void destroy_scope();

symbol_t *get_symbol_from_scope(char *key);

void print_table();