#include<stdio.h>
#include<string.h>
#include<stdlib.h>
// На вход функции подается строка — права доступа к файлу в формате rwx (9 знаков). 
// Указатель может принимать значение NULL.
// Функция должна вернуть соответствующее значение прав доступа, либо -1, если строка не является корректной записью прав доступа.
// Корректная запись состоит только из символов '-', 'r', 'w', 'x', расположенных в правильном порядке.
// Указание: используйте строку-образец "rwxrwxrwx" для разбора входной строки. В функции не должны использоваться константы 3, 8, 9, 10 (даже в виде enum или #define). 
// Используйте битовые операции.
// Поскольку строка-образец является константной, ее длину можно вычислить за константное время на этапе компиляции с помощью sizeof.
//  В функции может использоваться не более одного вызова strlen.
int parse_rwx_permissions(const char *str){
    int error = -1;
    if (str == NULL){
        return error;
    }
    int j = 0;
    int number = 0;
    while (str[j] != '\0' && str[j] != '\n'){
        if (str[j] != 'r' && str[j] != '-'){
            return error;
        }
        number <<= 1;
        if (str[j] == 'r'){
            number++;
        }
        j++;
        if (str[j] != 'w' && str[j] != '-'){
            return error;
        }
        number <<= 1;
        if (str[j] == 'w'){
            number++;
        }
        j++;
        if (str[j] != 'x' && str[j] != '-'){
            return error;
        }
        number <<= 1;
        if (str[j] == 'x'){
            number++;
        }
        j++;
    }
    char curr[10] = "rwxrwxrwx\0";
    //printf("%d %d\n", j, strlen(curr));
    if (j != strlen(curr)){
        return error;
    }
    return number;
}
// int main(){
//     char* str_true = (char*)malloc(10);
//     str_true[0] = 'r';
//     str_true[1] = 'w';
//     str_true[2] = 'x';
//     str_true[3] = 'r';
//     str_true[4] = 'w';
//     str_true[5] = 'x';
//     str_true[6] = 'r';
//     str_true[7] = 'w';
//     str_true[8] = 'x';
//     str_true[9] ='\0';
//     printf("%o\n", parse_rwx_permissions(str_true));
//     return 0;
// }