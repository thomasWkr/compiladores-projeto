#include "table.h"
#ifndef ASSEMBLER_HELPER
#define ASSEMBLER_HELPER

#define MAX_NAME_LEN 128

// Init labels and function label counters
void init_counters2();

// Generate next valid label
const char *next_label2();

// Generate next valid register
char *get_reg();

// Restart to first register
void restart_regs();

// This function returns assembly code
char *generate_individual_code(const char *operation, const char *oper1, const char *oper2);

// This function returns flux controll code based on iloc_operation
char *generate_individual_flux_code(char *operation, char *temp, const char *label_true, const char *label_false);

// This function returns the global definition in assembly of the passed variable
char *generate_global_var_segment(char *label, type_t type, nature_t nature, char *value);

// This function returns the init definition of a global function in assembly
char *generate_global_function_init_segment(char *label);

// This function returns the end definition of a global function in assembly
char *generate_global_function_end_segment();

// This function sets the global variable return_label to the value passed
void set_return_label(char *label);

// This function returns the global variable return_label
char *get_return_label();

#endif // ASSEMBLER_HELPER