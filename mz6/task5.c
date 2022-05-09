#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
// В аргументах командной строки задается путь к каталогу.

// На стандартный поток вывода напечатайте количество файлов в заданном
//каталоге (без подкаталогов), удовлетворяющих следующему условию:
// Файл регулярный.
// Файл доступен на выполнение текущему пользователю.
// Его имя заканчивается на суффикс .exe.

int main(int argc, char** argv){
    DIR* dir;
    if ((dir = opendir(argv[1])) == NULL){
        return 0;
    }
    long long count = 0;
    struct dirent* dir_info;
    struct stat file_info;
    char string[255];
    while ((dir_info = readdir(dir)) != NULL){
        if (snprintf(string, 255, "%s/%s", argv[1], dir_info->d_name) < 255){
            stat(string, &file_info);
            if (access(string, X_OK) == 0 &&
                    (S_ISREG(file_info.st_mode)) && 
                    (strcmp(&string[strlen(string) - 4], ".exe") == 0)){
                count++;
            }
        }
    }
    printf("%lld\n", count);
    closedir(dir);

    return 0;
}