#include "table.h"
#ifndef ASSEMBLER_HELPER
#define ASSEMBLER_HELPER
/*
generate this block per symbol
.text
    .globl	a
    .data
    .align 4
    .type	a, @object
    .size	a, 4
a:
    .long	3
    .text
*/
char *generate_var_data_segment(char *label, type_t type, nature_t nature, char *value);
#endif // ASSEMBLER_HELPER