#include "errors.h"
#include "codelist.h"
#include "assembler_helper.h"

// Stack of scopes
typedef struct scope
{
    scope_type type;
    symbol_t *symbol_table;
    int current_offset;
    struct scope *next_scope;
} scope;

// Stack for arguments temporaries
typedef struct args_stack
{
    parameters_t *args_list;
    struct args_stack *next_args_list;
} args_stack;

// Adds a scope to the top of the stack
void create_scope(int scope_type);

// Adds a symbol to the top scope symbol table
void update_table(content_t *content, char *key, int symbol_size);

// Delete and frees the top scope
void destroy_scope();

scope *get_scope();

void update_current_offset();

// Checks if a symbol is declared and used right
void check_declared(value_t *data, char *key);

// Adds an arguments list to the top of the args stack
void create_args_list();

// Adds an argument to the top arguments list
void update_args_list(type_t args_type);

// Delete and frees the top arguments list
void destroy_args_list();

// Compares the types of an arguments list to the top args stack list
void compare_args(parameters_t *args_function, value_t *data);

// Compares two symbol types
void compare_type(type_t type1, type_t type2, int line_number);

// Search for a symbol in the scopes stack
symbol_t *get_symbol_from_stack(char *key);

// Returns latest symbol in global scope
symbol_t *get_latest_function();

void print_scopes();