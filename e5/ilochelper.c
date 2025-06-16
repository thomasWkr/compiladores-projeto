#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ilochelper.h"

#define MAX_NAME_LEN 32

static int temp_counter = 0;
static int label_counter = 0;

static char buffer[MAX_NAME_LEN];

void init_counters()
{
    temp_counter = 0;
    label_counter = 0;
}

const char *next_temp()
{
    snprintf(buffer, MAX_NAME_LEN, "r%d", temp_counter++);
    return buffer;
}

const char *next_label()
{
    snprintf(buffer, MAX_NAME_LEN, "L%d", label_counter++);
    return buffer;
}
