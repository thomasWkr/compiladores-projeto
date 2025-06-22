#include "ilochelper.h"
#include <stddef.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define MAX_STR_LEN 256

typedef struct CodeNode
{
    char code[MAX_STR_LEN];
    struct CodeNode *next;
} CodeNode;

// Initialize the list (set head to NULL)
static inline void init_list(CodeNode **head)
{
    *head = NULL;
}

// Append a string to the list
void append(CodeNode **head, char *str);

// Print the list
void print_list(CodeNode *head);

// Free the list
void free_list(CodeNode **head);

// Concatenate two lists: list_a = list_a + list_b, list_b becomes empty
void concat_lists(CodeNode **list_a, CodeNode **list_b);

// Adds the labe to the instruction on top
void add_label(CodeNode **head, const char *label);

#endif