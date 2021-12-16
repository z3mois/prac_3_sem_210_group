#include<stdio.h>
#include<stdlib.h>
/*
Эту задачу сделать до 02.10 к следующему выходу на машины.
Написать функцию чтения строки со стандартного ввода.
Признак конца строки — символ перевода строки '\n' или признак конца файла EOF (проверять оба).
Длина строки неизвестна, считаем, что любой приготовленный буфер может оказаться мал.
Системные ограничения не рассматриваем. Функция должна возвращать указатель на введенную строку.
Для добавления памяти под строку использовать фукцию realloc.
Дополнительную память выделять блоками, длина блока должна быть кратна 4 (4, 8, 16,...),
задать эту длину с помощью #define. Для отладки лучше взять небольшой блок, например, 4 байта.
Запрещено использовать функцию realloc для добавления памяти по 1 байту, т.к. это крайне неэффективно.
Написать функцию main(), в которой продемонстрировать работу функции ввода.
*/
#define bufferok 4

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
int main() {
    char *string = ReadStr();
    printf("%s", string);
    free(string);
    return 0;
}
