#include "codelist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append(CodeNode **head, char *str)
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

void print_list(CodeNode *head)
{
    CodeNode *current = head;
    while (current)
    {
        printf("%s\n", current->code);
        current = current->next;
    }
}

void free_list(CodeNode **head)
{
    CodeNode *current = *head;
    while (current)
    {
        CodeNode *temp_node = current;
        current = current->next;
        free(temp_node);
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

void add_label(CodeNode **head, const char *label)
{
    int max_label = MAX_STR_LEN / 2 - 3;
    int max_code = MAX_STR_LEN - max_label - 3;
    char temp[MAX_STR_LEN];

    if (head && *head && label)
    {
        snprintf(temp, MAX_STR_LEN, "%.*s: %.*s", max_label, label, max_code, (*head)->code);
        strncpy((*head)->code, temp, MAX_STR_LEN - 1);
        (*head)->code[MAX_STR_LEN - 1] = '\0';
    }
    else if (head && label)
    {
        snprintf(temp, MAX_STR_LEN, "%.*s: nop", max_label, label);
        append(head, temp);
    }
}
