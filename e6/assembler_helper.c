#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler_helper.h"

char *generate_var_data_segment(char *label, type_t type, nature_t nature, char *value)
{
    char *buffer = malloc(512);
    if (!buffer)
        return NULL; // KABUM

    int size = 4;
    char *type_id = ".long";
    if (nature == FUNCTION)
    {
        snprintf(buffer, 512,
                 "    .text\n"
                 "    .globl\t%s\n"
                 "    .type\t%s, @function\n"
                 "%s:\n",
                 label, label, label);
    }
    else if (nature == ID)
    {
        snprintf(buffer, 512,
                 "    .text\n"
                 "    .globl\t%s\n"
                 "    .data\n"
                 "    .align 4\n"
                 "    .type\t%s, @object\n"
                 "    .size\t%s, %d\n"
                 "%s:\n"
                 "    %s\t%s\n"
                 "    .text\n",
                 label, label, label, size,
                 label,
                 type_id,
                 value);
    }

    return buffer;
}
