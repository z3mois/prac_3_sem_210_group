#pragma once

#include <stdio.h>
#include <stdlib.h>

struct _linked_list_node;
typedef struct _linked_list_node linked_list_node;
struct _linked_list_node
{
    void *value;
    linked_list_node *prev;
    linked_list_node *next;
};

struct _linked_list
{
    int count;
    linked_list_node *head;
    linked_list_node *tail;
};
typedef struct _linked_list linked_list;

void linked_list_init(linked_list *l)
{
    l->count = 0;
    l->head = NULL;
    l->tail = NULL;
}

void linked_list_add(linked_list *l, void *value)
{
    if (l->head == NULL) {
        l->head = l->tail = (linked_list_node *)malloc(sizeof(linked_list_node));
        l->tail->value = value;
        l->tail->prev = NULL;
        l->tail->next = NULL;
        l->count++;
        return;
    }
    l->tail->next = (linked_list_node *)malloc(sizeof(linked_list_node));
    l->tail->next->value = value;
    l->tail->next->prev = l->tail;
    l->tail->next->next = NULL;
    l->tail = l->tail->next;
    l->count++;
}

linked_list_node *linked_list_add_before(linked_list *l, linked_list_node *node, void *value)
{
    linked_list_node *prev = node->prev;
    node->prev = (linked_list_node *)malloc(sizeof(linked_list_node));
    node->prev->value = value;
    node->prev->prev = prev;
    node->prev->next = node;
    if (node == l->head)
        l->head = node->prev;
    l->count++;
    return node->prev;
}

linked_list_node *linked_list_add_after(linked_list *l, linked_list_node *node, void *value)
{
    linked_list_node *next = node->next;
    node->next = (linked_list_node *)malloc(sizeof(linked_list_node));
    node->next->value = value;
    node->next->prev = node;
    node->next->next = next;
    if (node == l->tail)
        l->tail = node->next;
    l->count++;
    return node->next;
}

void linked_list_remove(linked_list *l, linked_list_node *node)
{
    if (node->prev != NULL)
        node->prev->next = node->next;
    if (node->next != NULL)
        node->next->prev = node->prev;
    if (l->head == node)
        l->head = node->next;
    if (l->tail == node)
        l->tail = node->prev;
    free(node);
    l->count--;
}

int linked_list_iterate(linked_list *l, linked_list_node *(*f)(linked_list *, linked_list_node *))
{
    linked_list_node *node = l->head, *tmp;
    while (node != NULL) {
        if ((tmp = f(l, node)) == node)
            return -1;
        node = tmp;
    }
    return 0;
}

int linked_list_iterate_back(linked_list *l, linked_list_node *(*f)(linked_list *, linked_list_node *))
{
    linked_list_node *node = l->tail, *tmp;
    while (node != NULL) {
        if ((tmp = f(l, node)) == node)
            return -1;
        node = tmp;
    }
    return 0;
}

void linked_list_free(linked_list *l)
{
    linked_list_node *node = l->head;
    while (node != NULL) {
        linked_list_node *tmp = node->next;
        free(node);
        node = tmp;
    }
    l->count = -1;
}

/*linked_list_node *show(linked_list *l, linked_list_node *node)
{
    printf("%d, ", *(int *)node->value);
    return node->next;
}

int main()
{
    int a[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    linked_list l;
    linked_list_init(&l);
    for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        linked_list_add(&l, a + i);
    linked_list_iterate(&l, show);
    printf("\n");
    linked_list_free(&l);
}*/
