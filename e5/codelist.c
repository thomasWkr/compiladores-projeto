#include "codelist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append(CodeNode **head, const char *str)
{
    CodeNode *new_node = malloc(sizeof(CodeNode));
    if (!new_node)
    {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }
    strncpy(new_node->code, str, MAX_STR_LEN - 1);
    new_node->code[MAX_STR_LEN - 1] = '\0';
    new_node->next = NULL;

    if (!*head)
    {
        *head = new_node;
    }
    else
    {
        CodeNode *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }
}

void print_list(const CodeNode *head)
{
    const CodeNode *current = head;
    while (current)
    {
        printf("%s -> ", current->code);
        current = current->next;
    }
    printf("NULL\n");
}

void free_list(CodeNode **head)
{
    CodeNode *current = *head;
    while (current)
    {
        CodeNode *temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

void concat_lists(CodeNode **list_a, CodeNode **list_b)
{
    if (!*list_b)
        return;
    if (!*list_a)
    {
        *list_a = *list_b;
    }
    else
    {
        CodeNode *current = *list_a;
        while (current->next)
            current = current->next;
        current->next = *list_b;
    }
    *list_b = NULL;
}
