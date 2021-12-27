#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>

/*
Двустороннее взаимодействие с использованием каналов.
В командной строке передается имя файла. 
Процесс-отец создает «сына», открывает файл и
передает «сыну» по строке. 
«Сын» читает строку, реверсирует ее, оставляя на месте ‘\n’, если
он есть, и возвращает измененную строку «отцу». 
Тот читает ее и выводит в файл. В
результате должен быть изменен исходный файл.
*/

#define max_size 1024

void reverse(char* string, int string_length){
    char temp = 0;
    for(int i = 0; i < string_length / 2; i++){
        temp = string[i];
        string[i] = string[string_length - 1 - i];
        string[string_length - 1 - i] = temp;
    }
}
int main(int argc, char** argv){
    if (argc != 2){
        perror("error with arguments\n");
        return 0;
    }
    int fd[2], fe[2];
    pipe(fd);
    pipe(fe);
    pid_t pid;
    if ((pid = fork()) == -1){
        perror("error with create fork\n");
        return 0;
    }
    if (pid == 0){
        char string[max_size];// max strlen;
        int string_length = 0, flag = 1;
        close(fd[1]);// why cycling without this close?
        char temp = 0;
        while(read(fd[0],string, sizeof(string))){
            int string_length = strlen(string);
            if (string[string_length - 1] == '\n'){
                string_length--;
            }
            reverse(string, string_length);
            write(fe[1], string, max_size);
        }
        exit(0);
    }
    else{
        FILE* f;
        f = fopen(argv[1], "r+");
        if (f == NULL){
            perror("error with open file\n");
            wait(NULL);
            return 0;
        }
        char string[max_size];
        while (fgets(string, max_size, f) != 0){
            write(fd[1], string, max_size);
            read(fe[0], string, max_size);
            fseek(f, -strlen(string), SEEK_CUR);
            fprintf(f, "%s", string);
        }
        close(fd[0]);
        close(fd[1]);
        close(fe[0]);
        close(fe[1]);
        wait(NULL);
        fclose(f);
    }
    return 0;
}
