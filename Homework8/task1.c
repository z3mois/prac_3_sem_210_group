#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#define block 4
/*
Написать функцию с переменным количеством параметров, 
которая работает с заданным файлом как команда uniq - 
позволяет при выводе на экран сократить до одной подряд 
идущие одинаковые строки. Обязательный параметр функции — 
имя файла, необязательные — опции , любая из них (в том числе и все)
 при запуске функции может отсутствовать..
Опции команды uniq :
-c – перед каждой выводимой строкой помещать ее кратность во входном файле.
-d - подавить вывод неповторяющихся строк.
-u - выводить только уникальные строки.
-s число - при сравнении игнорировать заданное число начальных символов строки.
В main имя файла (или имя файла и опции) передается в командной строке. 
Продемонстрировать работу функции.
*/
char* inputstr(FILE* f, int* check){
    int buffer = block;
    char* answer = (char*) malloc(block*sizeof(char));
    int count = 0;
    char c;
    while (fscanf(f, "%c", &c) != -1){
        if (c != '\n'){
            *(answer + count ) =c;
            count++;
            if ((buffer + count) % block == 0){
                buffer += block;
                answer = (char*) realloc(answer, buffer*sizeof(char));
            }
        }
        else {
            *(answer + count) = '\0';
            break;
        }

    }
    if (c != '\n')
    if (fscanf(f, "%c", &c) == -1){
        *check = 0;
    }
    return answer;
}
// int multi(FILE* input, char* temp_str, int* check, int s){
//     int mul = 1;
//     for (int j = 0; j < size; j++){
//         if (j != i){
//             if (strlen(str) > s && strlen(mas[j]) > s){
//                 if(strcmp(mas[j] + s, str + s) == 0){
//                     mul++;
//                 }
//             }
//             else if(strlen(str) <= s && strlen(mas[j]) <= s){
//                 mul++;
//             }
//         }
//     }
//     return mul;
// }

int multi(FILE* input, char* temp_str, int* check, int s, int* len){
    int mul = 1;
    int flag = 1;
    char* curr;
    int check_for_multi = 1;
    int flag_for_enter;
    char* n = (char*)malloc(1);
    n[0] = '\0';
    int prev = 0;
    while (check_for_multi && flag){
        curr = inputstr(input, &check_for_multi);
        if (strcmp(curr, n) == 0){
            prev = flag_for_enter = flag_for_enter;
            flag_for_enter = 1;
        }
        else{
            prev = flag_for_enter = flag_for_enter;
            flag_for_enter = 0;
        }
        //printf("aaa %s %s\n", curr + s, temp_str + s);
        if (strlen(curr) >= s && strlen(temp_str) >= s){
            if (strcmp(curr + s, temp_str + s) == 0){
                mul++;
                flag = 1;
                //printf("ucl11\n");
            }
            else {
                flag = 0;
                //printf("ucl12\n");
            }
        }
        else if (strlen(curr) <= s && strlen(temp_str) <= s){
            mul++;
            flag = 1;
            //printf("ucl2\n");
        }
        else {
            flag = 0;
        }
        *len = strlen(curr);
        free(curr);
    }
    if (check_for_multi == 0 && flag_for_enter == 1 && prev == 1){
        mul--;
    }
    free(n);
    *check = check_for_multi;
    //printf("\n");
    return mul;
}

void my_uniq(char* file_name, int parametrs, ...){
    FILE* input = fopen(file_name, "r+");
    if (input == NULL){
        perror("can't open file");
        return;
    }
    char c = 0, d = 0, u = 0;
    int s = 0;
    va_list ap;
    va_start(ap, parametrs);
    char* temp;
    for(int i = 0; i < parametrs; i++){
        temp = (va_arg(ap, char*));
        if (temp[1] == 'c'){
            c = 1;
        }
        else if (temp[1] == 'd'){
            d = 1;
        }
        else if (temp[1] == 'u'){
            u = 1;
        }
        else {
            s = atoi(temp);
        }
    }
    s = -s;
    va_end(ap);
    int check = 1;
    char* temp_str;
    char* curr;
    int len;
    int multi_str = 0;
    char* last;
    int count = 0;
    printf("example:./a.out inp.txt -c -d -2\n");
    printf("c = %d d =  %d u = %d s = %d\n", c, d, u, s);
    if (d == 1 &&  u == 1){
        fclose(input);
        return;
    }
    while(check){
        temp_str = inputstr(input, &check);
        //printf("%s", temp_str)
        //printf("aaa %s\n", temp_str);
        if (count == 0 && check == 0 && strcmp(temp_str, "\0") == 0){
        }
        else {
        if (d){
            multi_str = multi(input, temp_str, &check, s, &len);
            if (multi_str > 1){
                if (c) {
                    printf("%d %s\n", multi_str, temp_str);
                }
                else {
                    printf("%s\n", temp_str);                        
                }
            }
        } else 
        if (u){
            multi_str = multi(input, temp_str, &check, s, &len);
            if (multi_str == 1){
                if (c) {
                    printf("%d %s\n", multi_str, temp_str);
                }
                else {
                    printf("%s\n", temp_str);                        
                }
            } 
            if (check == 0 && multi_str == 1){
                fseek(input, -len - 1, SEEK_CUR);
                curr = inputstr(input, &check);
                last = inputstr(input, &check);
                //printf("aaa %d %d", *last, '\n');
                char* bad = malloc(1);
                bad[0] = 0;
                if (strcmp(last, bad) != 0){
                if (c) {
                    printf("1 %s\n", last);
                }
                else {
                    printf("%s\n", last);                        
                }
                }
                free(bad);
                free(curr);
                free(last);
            }             
        }
        else {
            multi_str = multi(input, temp_str, &check, s, &len);
            if (c){
                printf("%d %s\n", multi_str, temp_str);
            }
            else 
                printf("%s\n", temp_str);
        }
        }
        //prev_str = &temp_str;
        fseek(input, -len - 1, SEEK_CUR);
        free(temp_str);
        count ++;
    }
    fclose(input);
}
int main(int argc, char** argv){
    if (argc < 2){
        perror("malo parametrov");
        return 0;
    }
    if (argc == 2){
        my_uniq(argv[1], 0);
        return 0;
    }
    if (argc == 3){
        my_uniq(argv[1], 1, argv[2]);
        return 0;
    }
    if (argc == 4){
        my_uniq(argv[1], 2, argv[2], argv[3]);
        return 0;
    }
    if (argc == 5){
        my_uniq(argv[1], 3, argv[2], argv[3], argv[4]);
        return 0;
    }
    if (argc == 6){
        my_uniq(argv[1], 4, argv[2], argv[3], argv[4], argv[5]);
        return 0;
    }
    return 0;
}
