#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*
Имя файла и целое N передаются в командной строке.
Удалить из файла все строки, длина которых больше N. 
Символ перевода строки '\n' в подсчете длины не учитывать, 
длину пустой строки считать равной 0. 
Длина строк в файле не ограничена.
 Функцию чтения длинной строки в этой программе не использовать
*/
int main(int argc, char** argv){

    //printf("please, write in the end of str symbol '%\%n':\n");
    //printf("%d\n", n);
    int n = atoi(argv[1]);
    int len = 0 , total_len = 0;
    char c, temp;
    FILE* f = fopen(argv[2], "r");
    if (f == NULL){
        return 0;
    }
    FILE* answer = tmpfile();
    FILE* curr = tmpfile(); 
    printf("your file without strings sizes > n:\n");
    if (curr != NULL && answer != NULL){  
    while (!feof(f)) {
        fscanf(f, "%c", &c);
        total_len++;
        //printf("%c", c);
        if (len > n) {
            //printf("%d\n", len);
            while(c!= '\n' && !feof(f)){
                fscanf(f, "%c", &c);
            }
            fclose(curr);
            curr = tmpfile();
            len = 0;
        }
        else {
            //printf("%c", c);
            if (c == '\n' || c == EOF || feof(f)){
                total_len--;
                //printf("%d\n", len);
                if (len > 0) {
                fseek(curr, 0, SEEK_SET);
                for(int i = 0; i < len ;i++) {
                    fscanf(curr, "%c", &temp);
                    //printf("%c", temp);
                    fputc(temp, answer);
                    //fprintf(answer, "%c", temp);
                    //printf("%c", temp);
                }
                fprintf(answer, "%c", '\n');
                }
                if (len == 0 && !feof(f)){
                    fprintf(answer, "%c", '\n');  
                }  
                fclose(curr);
                curr = tmpfile();
                len = 0;
                //printf("\n");
            }
            else {
                //fprintf(curr, "%c", c);
                fputc(c, curr);
                //fscanf(curr, "%c", &temp);
                //printf("%c", c);
                len++;
                //printf("%d", len);
                //printf("%c", c);
            }
        }
    }
    }
    //printf("\n");
    if (len > 0) {
        fseek(curr, 0, SEEK_SET);
        for (int i = 0; i < len; i++) {
            fscanf(curr, "%c", &temp);
            fputc(temp, answer);
        }
        fclose(curr);
    }
    fclose(f);
    f = fopen("input.txt", "w");
    fseek(answer, 0, SEEK_SET);
    //printf("%d\n", total_len);
    if (total_len)
    while(!feof(answer)){
        fscanf(answer, "%c", &temp);
        if (temp != -2 && temp != 0) 
            fprintf(f, "%c", temp);
        printf("%c", temp);
    }
    fclose(answer);
    fclose(f);
    return 0;
}
