#pragma once

#include <stdio.h>
#include <ctype.h>

int skip_quotes(char *str, int len, int *index)
{
    if (str[*index] != '"')
        return 0;
    (*index)++;
    while (*index < len && (str[*index] != '"'))// || *index > 0 && str[*index - 1] == '\\'))
        (*index)++;
    (*index)++;
    return *index <= len;
}

int skip_brackets(char *str, int len, int *index)
{
    if (str[*index] != '(')
        return 0;
    (*index)++;
    int count = 1;
    while (*index < len && count > 0) {
        if (str[*index] == '"' && !skip_quotes(str, len, index))
            return 0;
        else if (str[*index] == '(')
            count++;
        else if (str[*index] == ')')
            count--;
        (*index)++;
    }
    return count == 0;
}

int isoperator(char *term)
{
    return term[0] == '>' || term[0] == '<' || term[0] == '&' || term[0] == '|' || term[0] == ';';
}

int isoutput(char *term)
{
    return term[0] == '>' && strlen(term) == 1;
}

int isoutputadd(char *term)
{
    return term[0] == '>' && strlen(term) == 2;
}

int isinput(char *term)
{
    return term[0] == '<';
}

int isconveyor(char *term)
{
    return term[0] == '|' && strlen(term) == 1;
}

int isor(char *term)
{
    return term[0] == '|' && strlen(term) == 2;
}

int isbackground(char *term)
{
    return term[0] == '&' && strlen(term) == 1;
}

int isand(char *term)
{
    return term[0] == '&' && strlen(term) == 2;
}

int issemicolon(char *term)
{
    return term[0] == ';';
}

int isbrackets(char *term)
{
    return term[0] == '(';
}

int isquotes(char *term)
{
    return term[0] == '"';
}

int iscommand(char *term)
{
    return !isoperator(term) && !isbrackets(term);
}

int isarg(char *term)
{
    return iscommand(term);
}

int read_term(char *cmd, int len, int *index, char **term)
{
    *term = NULL;
    int start = *index;

    // skip all spaces
    while (start < len && isspace(cmd[start]))
        start++;
    if (start >= len)
        return 0;

    int i = start + 1;
    if (cmd[start] == '>') {
        if (i < len && cmd[i] == '>')
            i++;
    } else if (cmd[start] == '<') {

    } else if (cmd[start] == '(') {
        i--;
        if (!skip_brackets(cmd, len, &i))
            return -1;
    } else if (cmd[start] == ')') {
        return -1;
    } else if (cmd[start] == '"') {
        i--;
        if (!skip_quotes(cmd, len, &i))
            return -1;
    } else if (cmd[start] == '&') {
        if (i < len && cmd[i] == '&')
            i++;
    } else if (cmd[start] == '|') {
        if (i < len && cmd[i] == '|')
            i++;
    } else if (cmd[start] == ';') {

    } else {
        while (i < len && !isspace(cmd[i]) && cmd[i] != '(' && cmd[i] != ')' && cmd[i] != '"' && !isoperator(cmd + i))
            i++;
    }

    *index = i;
    *term = (char *)malloc(i - start + 1);
    strncpy(*term, cmd + start, i - start);
    (*term)[i - start] = '\0';
    return i - start;
}
