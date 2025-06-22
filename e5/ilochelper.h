#ifndef ILOC_HELPER_H
#define ILOC_HELPER_H

//
void init_counters();

//
const char *next_temp();

//
const char *next_label();

//
char *generate_iloc_individual_code(const char *iloc_operation, const char *oper1, const char *oper2, const char *dest1, const char *dest2);

//
char *generate_iloc_flux_code(const char *iloc_operation, const char *oper1, const char *oper2, const char *dest1, const char *dest2);

#endif
