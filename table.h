// ----------------- ENUMS --------------------
typedef enum
{
    INT,
    FLOAT,
} type_t;

typedef enum
{
    LITERAL,
    ID,
    FUNCTION,
} nature_t;

// ----------------- STRUCTS ------------------
typedef struct
{
    int params_count;
    type_t *args_type;
} parameters_t;

typedef struct valor
{
    char *lexeme;
    int line_number;
    nature_t nature;
} value_t;

typedef struct
{
    type_t type;
    nature_t nature;
    parameters_t *args;
    value_t *data;
} content_t;

// Symbol Table struct
typedef struct symbol_t
{
    char *key;
    content_t *content;
    struct symbol_t *next_symbol;
} symbol_t;

// ---------------- FUNCTIONS -----------------

value_t *create_lexic_value(nature_t nature, char *lexeme, int line_number);

content_t *create_content(type_t type, nature_t nature, parameters_t *args, value_t *data);

symbol_t *new_table(content_t *content, char *key);

symbol_t *add_symbol(content_t *content, symbol_t *previous_symbol, char *key);

symbol_t *get_symbol(symbol_t *first_symbol, char *key);

void free_table(symbol_t *symbol_table);

parameters_t *add_arg(parameters_t *params_function, type_t arg);

void free_args(parameters_t *args);

const char *type_to_str(type_t type);

const char *nature_to_str(nature_t nature);