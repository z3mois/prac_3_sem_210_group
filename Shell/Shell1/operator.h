#pragma once

#include "dynamic_array.h"

enum op_type
{
    op_term,
    op_simple_command,
    op_io,
    op_conveyor,
    op_command,
    op_and,
    op_or,
    op_background,
    op_root
};

static const char *operator_name[] = {
    "term", // непонятно что
    "simple_command",//
    "io",// >< >>
    "conveyor",
    "command",// конверйе + > или () 
    "and",
    "or",
    "background",
    "root"
};

struct _operator;
typedef struct _operator operator;
struct _operator
{
    enum op_type type;
    char *text;
    dynamic_array args;
};

operator *operator_new(enum op_type type, char *text, operator **args, int args_count)
{
    operator *result = (operator *)malloc(sizeof(operator));
    result->type = type;
    result->text = text;
    dynamic_array_init(&(result->args));
    for (int i = 0; i < args_count; i++)
        dynamic_array_add(&(result->args), args[i]);
    return result;
}

void operator_free(operator *op);
int free_args(dynamic_array *arr, void *arg)
{
    operator_free((operator *)arg);
    free(arg);
    return 0;
} 

void operator_free(operator *op)
{
    if (op->text != NULL)
        free(op->text);

    dynamic_array_iterate(&(op->args), free_args);
    dynamic_array_free(&(op->args));
}

void operator_add_arg(operator *op, operator *arg)
{
    dynamic_array_add(&(op->args), arg);
}
