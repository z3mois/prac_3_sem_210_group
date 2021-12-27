#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void * dynamic_array_element;

struct _dynamic_array
{
    dynamic_array_element *value;
    int count;
    int size;
};
typedef struct _dynamic_array dynamic_array;

void *_dynamic_array_add(dynamic_array_element *array, int *count, int *size, dynamic_array_element value)
{
    if (array == NULL) {
        *size = 1;
        array = malloc(*size * sizeof(dynamic_array_element));
        if (array == NULL)
            return NULL;
        array[0] = value;
        *count = 1;
        return array;
    } else if (*count == *size) {
        *size *= 2;
        void *arraysv = array;
        array = realloc(array, *size * sizeof(dynamic_array_element));
        if (array == NULL)
            return arraysv;
    }
    array[*count] = value;
    (*count)++;
    return array;
}

void dynamic_array_init(dynamic_array *array)
{
    array->value = NULL;
    array->count = 0;
    array->size = 0;
}

void dynamic_array_add(dynamic_array *array, void *value)
{
    array->value = _dynamic_array_add(array->value, &(array->count), &(array->size), value);
}

int dynamic_array_iterate(dynamic_array *array, int (*f)(dynamic_array *, void *))
{
    for (int i = 0; i < array->count; i++)
        if (f(array, array->value[i]) != 0)
            return -1;
    return 0;
}

void dynamic_array_free(dynamic_array *array)
{
    if (array->value != NULL)
        free(array->value);
    array->count = -1;
    array->size = -1;
}

/*int show(dynamic_array *arr, void *value)
{
    printf("%d, ", *(int *)value);
    return 0;
}

int main()
{
    int a[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    dynamic_array arr;
    dynamic_array_init(&arr);
    for (int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
        dynamic_array_add(&arr, a + i);
    dynamic_array_iterate(&arr, show);
    printf("\n");
    dynamic_array_free(&arr);
}*/
