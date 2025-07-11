#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler_helper.h"

static int func_counter = 0;
static int label_counter = 0;
static int register_counter = 0;

char regs[6][10] = {"%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi"};

void init_counters2()
{
    func_counter = 0;
    label_counter = 0;
    register_counter = 0;
}

const char *next_label2()
{
    char *buffer = malloc(MAX_NAME_LEN);
    snprintf(buffer, MAX_NAME_LEN, ".L%d", label_counter++);
    return buffer;
}

char *get_reg()
{
    register_counter++;
    return regs[register_counter - 1];
}

void restart_regs()
{
    register_counter = 0;
}

char *generate_individual_code(const char *operation, const char *oper1, const char *oper2)
{
    static char buffer[MAX_NAME_LEN];
    if (strcmp(operation, "nop") == 0)
    {
        snprintf(buffer, MAX_NAME_LEN, "nop");
    }
    else
    {
        if (oper1 && oper2)
        {
            snprintf(buffer, MAX_NAME_LEN, "    %s\t%s, %s", operation, oper1, oper2);
        }
        else if (oper1)
        {
            snprintf(buffer, MAX_NAME_LEN, "    %s\t%s", operation, oper1);
        }
        else
        {
            printf("NÚMERO INSUFICIENTE DE ARGUMENTOS NA OPERAÇÃO\n");
            return NULL;
        }
        return buffer;
    }
}

char *generate_individual_flux_code(char *operation, char *temp, const char *label_true, const char *label_false)
{
    static char buffer[MAX_NAME_LEN];
    if (strcmp(operation, "nop") == 0)
    {
        snprintf(buffer, MAX_NAME_LEN, "    nop");
    }
    else
    {
        if (strcmp(operation, "cmp"))
        {
            snprintf(buffer, MAX_NAME_LEN,
                     "    cmp\t0, %s\n"
                     "    jne\t%s\n"
                     "    jmp\t%s",
                     temp, label_true, label_false);
        }
        else if (strcmp(operation, "jmp"))
        {
            snprintf(buffer, MAX_NAME_LEN,
                     "    jmp\t%s",
                     label_true);
        }
        else
        {
            printf("NÚMERO INSUFICIENTE DE ARGUMENTOS NA OPERAÇÃO\n");
            return NULL;
        }
        return buffer;
    }
}

char *generate_global_var_segment(char *label, type_t type, nature_t nature, char *value)
{
    static char buffer[MAX_NAME_LEN];

    int size = 4;
    char *type_id = ".long";
    if (nature == ID)
    {
        snprintf(buffer, 512,
                 "    .text\n"
                 "    .globl\t%s\n"
                 "    .data\n"
                 "    .align\t4\n"
                 "    .type\t%s, @object\n"
                 "    .size\t%s, %d\n"
                 "%s:\n"
                 "    %s\t%s",
                 label, label,
                 label, size,
                 label,
                 type_id,
                 value);
    }

    return buffer;
}

char *generate_global_function_init_segment(char *label)
{
    static char buffer[MAX_NAME_LEN];

    snprintf(buffer, MAX_NAME_LEN,
             "    .text\n"
             "    .globl\t%s\n"
             "    .type\t%s, @function\n"
             "%s:\n"
             ".LFB%d:"
             "    pushq\t%%rbp\n"
             "    movq\t%%rsp, %%rbp",
             label, label, label, func_counter);
    return buffer;
}

char *generate_global_function_end_segment()
{
    static char buffer[MAX_NAME_LEN];
    snprintf(buffer, MAX_NAME_LEN,
             "    popq\t%%rbp"
             ".LFE%d:",
             func_counter);
    return buffer;
}
