#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilochelper.h"

static int temp_counter = 0;
static int label_counter = 0;

void init_counters()
{
    temp_counter = 0;
    label_counter = 0;
}

char *next_temp()
{
    char *buffer = malloc(MAX_NAME_LEN);
    snprintf(buffer, MAX_NAME_LEN, "r%d", temp_counter++);
    return buffer;
}

const char *next_label()
{
    char *buffer = malloc(MAX_NAME_LEN);
    snprintf(buffer, MAX_NAME_LEN, "L%d", label_counter++);
    return buffer;
}

char *generate_iloc_individual_code(const char *iloc_operation, const char *oper1, const char *oper2, const char *dest1, const char *dest2)
{
    static char buffer[MAX_NAME_LEN];
    if (strcmp(iloc_operation, "nop") == 0)
    {
        snprintf(buffer, MAX_NAME_LEN, "nop");
    }
    else
    {
        if (!oper1 && !dest1)
        {
            printf("NÚMERO INSUFICIENTE DE ARGUMENTOS NA OPERAÇÃO\n");
            return NULL;
        }
        else if (oper2 && dest2)
        {
            snprintf(buffer, MAX_NAME_LEN, "%s %s, %s => %s, %s", iloc_operation, oper1, oper2, dest1, dest2);
        }
        else if (oper2)
        {
            snprintf(buffer, MAX_NAME_LEN, "%s %s, %s => %s", iloc_operation, oper1, oper2, dest1);
        }
        else if (dest2)
        {
            snprintf(buffer, MAX_NAME_LEN, "%s %s => %s, %s", iloc_operation, oper1, dest1, dest2);
        }
        else if (oper1 && dest1)
        {
            snprintf(buffer, MAX_NAME_LEN, "%s %s => %s", iloc_operation, oper1, dest1);
        }
        else
        {
            printf("NÚMERO MUITO ALTO DE ARGUMENTOS NA OPERAÇÃO\n");
            return NULL;
        }
        return buffer;
    }
}

char *generate_iloc_flux_code(const char *iloc_operation, const char *oper1, const char *oper2, const char *dest1, const char *dest2)
{
    static char buffer[MAX_NAME_LEN];
    if (oper2)
    {
        snprintf(buffer, MAX_NAME_LEN, "%s %s, %s -> %s", iloc_operation, oper1, oper2, dest1);
    }
    else if (dest2)
    {
        snprintf(buffer, MAX_NAME_LEN, "%s %s -> %s, %s", iloc_operation, oper1, dest1, dest2);
    }
    else if (dest1)
    {
        snprintf(buffer, MAX_NAME_LEN, "%s -> %s", iloc_operation, dest1);
    }
    else
    {
        printf("NÚMERO INSUFICIENTE DE ARGUMENTOS NA OPERAÇÃO\n");
        return NULL;
    }
    return buffer;
}
