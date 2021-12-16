#include<stdio.h>
#include<stdlib.h>
#define bufferok 4
/*
Написать функцию, определяющую первое вхождение подстроки в строку.
Параметры — строка и подстрока, могут быть и пустыми.
Возвращать адрес начала этого вхождения, либо NULL, если подстрока в строку не входит. 
Стандартные функции для работы со строками использовать запрещено.
В main() ввести 2 строки, и вызывать функцию поиска, чтобы найти все вхождения подстроки в строку. 
Для каждого вызова выводить на экран результат как строку - «хвост» исходной, если вхождений больше нет, вывести сообщение.
*/
char* ReadStr() {
    int buffer = bufferok;
    char *answer =(char*)malloc(buffer * sizeof(char));
    char temp;
    int count = 0;
    while(scanf("%c", &temp) != -1) {
        if (temp != '\n'){
            *(answer + count) = temp;
            count++;
            if ((buffer+count)%bufferok == 0) {
                buffer += bufferok;
                answer = (char*)realloc(answer, buffer * sizeof(char));
            }
        }
        else{
            *(answer + count) = '\0';
            break;
        }
    }
    return answer;
}
char* find(char* inp, char* under){
    char* inp_= inp;
    char* under_ = under;
    int flag;
    while(*inp != '\0') {
        inp_= inp;
        under_ = under;
        flag = 0;
        while (*inp_++ == *under_++) {
            if (*inp_ == '\0' || *under_ == '\0'){
                flag = 1;
                break;
            }
        }
        if (*under_ == '\0' && flag){
            return inp;
        }
        if (*inp_ == '\0' && flag){
            return NULL;
        }
        inp++;
    }
    return NULL;
}
int main() {
    char *inp_string = ReadStr();
    //int length1 = count;
    //count = 0;
    char *under_string = ReadStr();
    //int length2 = count; 
    //printf("%s\n", inp_string);
    //printf("%s\n", under_string);
    //printf("%d\n", length1);
    //printf("%d\n", length2);

    //printf("%s ",inp_string);
    //printf("%s",under_string);
    char* temp = find(inp_string, under_string);
    if (temp == NULL){
        printf("Null");
    }
    while (1) {
        if (temp == NULL)
            break;
        printf("%s\n", temp);
        ++temp;
        temp = find(temp, under_string);
    }
    free(temp);
    free(inp_string);
    free(under_string);
    return 0;
}
