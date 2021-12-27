#include <termios.h>
#include <unistd.h>
#include <string.h>
#define block 4
/*
Написать программу, работающую подобно команде more, 
которая осуществляет вывод файла на экран порциями по нажатию 
клавиши <пробел> (можете выбрать любую).
Нажатие <q> - прервать работу (дальше не выводить файл).
more [-s] [-num] [+linenum] файл
Опции:
-s – предписывает заменять 
последовательность пустых строк одной пустой строкой.
-num - целое число задает количество выводимых за один раз строк.
Длина одной порции по умолчанию, 
т.е. в том случае, если эта опция не задана, 10 строк.
+linenum - начать вывод со строки с номером linenum (целое число >=1)
Порядок опций определен, любая из них (в том числе и все) 
при запуске программы может отсутствовать.
Примеры вызова: > ./my_more -s -6 +10 file

> ./my_more -16 file

> ./my_more file

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
            if ((buffer+count)%block == 0){
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
void readarg(int* s, int* num, int* linenum, int argc, char** argv){
    if (argc > 2) {
        for (int i = 1; i < argc - 1; i++){
            if ((argv[i][0] == '-') && (argv[i][1] == 's')){
                *s = 1;
            }
            else if (argv[i][0] == '-'){
                *num = atoi(argv[i] + 1);
            }
            else if (argv[i][0] =='+'){
                *linenum = atoi(argv[i] + 1);
            }
        }
    }
}

void my_more(int s, int num, int linenum, FILE* f){
    if (!feof(f)){
 
    if (num && s){
        //char* mas[num];
        int prev = 0, curr = 0, first = 1;
        int i = 0;
        char* temp;
        // temp = inputstr(f);
        // if (*temp == '\0'){
        //     prev = 1;
        //     free(temp);
        // }
        // else {
        //     mas[i] = temp;
        //     i++;
        //     prev = 0;
        // }
        int check = 1;
        A:while (i < num && check){
            temp = inputstr(f, &check);
            //printf("%d %d\n", check, i);
            if (*temp == '\0'){
                curr = 1;
                //free(temp);
            }
            else {
                curr = 0;
            }
            if (curr == 1 && prev == 1){
                prev = curr;
                free(temp);
                goto A;
            }else 
            if (curr == 0 && prev == 1){
                //printf("ok\n");
                char* o = (char*) malloc(1);
                *o = '\0';
                // mas[i] = o;
                //printf("%s\n", mas[i]);
                // mas[i+1] = temp;
                //printf("%s\n", mas[i+1]);
                printf("%s\n", o);
                free(o);
                printf("%s\n", temp);
                free(temp);
                i +=2;
            }
            else if (curr == 0 && prev == 0){
                //mas[i] = temp;
                printf("%s\n", temp);
                free(temp);
                //printf("%s\n", mas[i]);
                i++;
            }
            if (*temp == '\0') {
                free(temp);
            }
            prev = curr;
        }
    }
    else 
    if (num) {
        char* temp;
        int c;
        for (int i = 0; i < num; i++){
            temp = inputstr(f, &c);
            printf("%s\n", temp);
            free(temp);
        }
    }
    }
}




int main(int argc, char** argv){
    FILE* f = fopen(argv[argc - 1], "r");
    int s = 0;
    int num = 10;
    int linenum = 1;
    readarg(&s, &num, &linenum, argc, argv);
    printf("%d %d %d\n", s, num, linenum);
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt); 
    // сохранение текущего режима
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // изменение 
    char c;
    int check;
    if (linenum){
        char* str;
        for (int i = 1; i < linenum; i++){
            str = inputstr(f, &check);
            free(str);
        }
    }
    while (c != 'q') {
        c = getchar();
        printf("==%c\n", c);
        my_more(s, num, linenum, f);   
    }
    fclose(f);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // восстановление исходного режима
    return 0;
}
