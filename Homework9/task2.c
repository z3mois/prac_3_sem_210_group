#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
/*
Написать программу, которая выводит на экран список каталогов 
(директорий), содержащихся в текущем каталоге, и делает это рекурсивно,
 т.е. для всех вложенных каталогов. 
Если нет вложенных каталогов, вывести сообщение об этом.
Имя текущего каталога - «.»
Имя родительского каталога (на уровень выше) - «..»
Получить абсолютный путь к текущему рабочему каталогу - см. функция getcwd()
Переход в другой каталог — см. функция chdir()
*/
#define MAXDIR 5000
//typedef struct stat stat; why it's doesn't work?
typedef struct dirent dirent;
void all_dir(char* dir){
    char* next = (char*) malloc(MAXDIR * sizeof(char));
    dirent* info_about_dir;
    DIR* directory = opendir(dir);
    struct stat info_about_file;
    while ((info_about_dir = readdir(directory)) != NULL){
        if (strcmp(info_about_dir->d_name, ".") != 0 && strcmp(info_about_dir->d_name, "..") != 0){// ne ciclimcia
            stat(info_about_dir -> d_name, &info_about_file);
            if (info_about_file.st_mode & S_IFDIR){//check directory or file
                // why vscode don't see const S_IFDIR?
                printf("%s\n", info_about_dir -> d_name);
                if(chdir(info_about_dir -> d_name) == 0){
                    getcwd(next, MAXDIR);
                    all_dir(next);
                    chdir("..");  
                }
            }
        }
    }
    free(info_about_dir);
    closedir(directory);
    free(next);
    return;
}
int main(){
    char* dir = (char*)malloc(MAXDIR * sizeof(char));
    getcwd(dir, MAXDIR);
    printf("Your start directory is %s\n", dir);
    all_dir(dir);
    printf("That all directory in your directory\n");
    free(dir);
    return 0;
}
