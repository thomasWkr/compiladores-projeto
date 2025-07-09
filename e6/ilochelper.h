#ifndef ILOC_HELPER_H
#define ILOC_HELPER_H

#define MAX_NAME_LEN 128

// Init label and reg counters
void init_counters();

// Generate next valid temp
char *next_temp();

// Generate next valid label
const char *next_label();

// This function returns binary operands iloc code
char *generate_iloc_individual_code(const char *iloc_operation, const char *oper1, const char *oper2, const char *dest1, const char *dest2);

// This function returns flux controll code based on iloc_operation
char *generate_iloc_flux_code(const char *iloc_operation, const char *oper1, const char *oper2, const char *dest1, const char *dest2);

#endif
