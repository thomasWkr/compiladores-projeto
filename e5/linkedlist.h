#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define MAX_STR_LEN 256

typedef struct CodeNode
{
    char code[MAX_STR_LEN];
    struct CodeNode *next;
} CodeNode;

typedef struct
{
    CodeNode *head;
} LinkedList;

void init_list(LinkedList *list);
void append(LinkedList *list, const char *str);
void print_list(const LinkedList *list);
void free_list(LinkedList *list);
void concat_lists(LinkedList *lista_a, LinkedList *lista_b);

#endif