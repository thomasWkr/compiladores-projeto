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
    int offset;
    struct symbol_t *next_symbol;
} symbol_t;

// ---------------- FUNCTIONS -----------------

// Alloc a value_t
value_t *create_lexic_value(nature_t nature, char *lexeme, int line_number);

// Alloc a content_t
content_t *create_content(type_t type, nature_t nature, parameters_t *args, value_t *data);

// Creates a new Symbol Table
symbol_t *new_table(content_t *content, char *key, int offset);

// Adds a symbol to a Symbol Table
symbol_t *add_symbol(content_t *content, symbol_t *previous_symbol, char *key, int offset);

// Returns a symbol from a Symbol Table
symbol_t *get_symbol(symbol_t *first_symbol, char *key);

// Frees a Symbol Table
void free_table(symbol_t *symbol_table);

// Adds a parameter to the end of a parameter list
parameters_t *add_arg(parameters_t *params_function, type_t arg);

// Frees a parameter list
void free_args(parameters_t *args);

const char *type_to_str(type_t type);

const char *nature_to_str(nature_t nature);