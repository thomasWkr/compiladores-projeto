#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

void init_list(LinkedList *list)
{
    list->head = NULL;
}

void append(LinkedList *list, const char *str)
{
    CodeNode *new_node = (CodeNode *)malloc(sizeof(CodeNode));
    if (!new_node)
    {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    strncpy(new_node->code, str, MAX_STR_LEN - 1);
    new_node->code[MAX_STR_LEN - 1] = '\0';
    new_node->next = NULL;

    if (!list->head)
    {
        list->head = new_node;
    }
    else
    {
        CodeNode *current = list->head;
        while (current->next)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

void print_list(const LinkedList *list)
{
    CodeNode *current = list->head;
    while (current)
    {
        printf("%s -> ", current->code);
        current = current->next;
    }
    printf("NULL\n");
}

void free_list(LinkedList *list)
{
    CodeNode *current = list->head;
    while (current)
    {
        CodeNode *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
}

void concat_lists(LinkedList *lista_a, LinkedList *lista_b)
{
    if (!lista_b->head)
    {
        return;
    }

    if (!lista_a->head)
    {
        lista_a->head = lista_b->head;
    }
    else
    {
        CodeNode *current = lista_a->head;
        while (current->next)
        {
            current = current->next;
        }
        current->next = lista_b->head;
    }

    lista_b->head = NULL;
}
