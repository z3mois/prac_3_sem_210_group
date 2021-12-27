#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
/*
В командной строке передаются имена 2-ух исполняемых файлов, для каждого по 1
параметру. 
Также должен быть передан параметр, определяющий режим запуска — 
параллельно (1) или последовательно (2).
Командная строка выглядит примерно так:
>./a.out ./prog1 arg11 ./prog2 arg21 1
В зависимости от значения последнего параметра
запустить эти программы с использованием векторной передачи данных для последовательного
 исполнения ( работают строго одна после другой) .
или запустить эти программы с использованием передачи данных в виде списка 
для параллельного исполнения ( т. е. работают одновременно) .
Определить, как завершились программы, вывести эту информацию на экран 
( успешно ли (т. е. по exit), что вернул exit, и т.д.).
По окончании работы не должно остаться процессов «зомби».
Для тестирования напишите программу (достаточно одной), 
которая должна работать не слишком быстро и осуществлять вывод на экран
 (пусть N раз , где N — аргумент командной строки, выводит свой pid процесса), и запускайте ее.
*/
int main(int argc, char** argv){
    if (argc > 6 || argc < 6){
        perror("many or few parametrs\n");
        return 0;
    }
    if (atoi(argv[5]) == 1){
        pid_t pid1 = fork();
        if (pid1 == -1){
            perror("can't create process one\n");
            return 0;
        }
        if (pid1 == 0){
            execl(argv[1], argv[1], argv[2], NULL);
            perror("you have problem with file1 or arg1\n");
            exit(-1);
        }
        pid_t pid2= fork();
        if (pid2 == -1){
            perror("can't create process two\n");
            return 0;
        }
        if (pid2 == 0){
            execl(argv[3], argv[3], argv[4], NULL);
            perror("you have problem with file2 or arg2\n");
            exit(-1);
        }
        int status1, status2;
        waitpid(pid1, &status1, 0);
        waitpid(pid2, &status2, 0);
        if (WIFEXITED(status1) != 0){
            printf("procces one done with status = %d\n", WEXITSTATUS(status1));
        }
        else{
            perror("something wrong in proccess one\n");
            return 0;
        }
        
        if (WIFEXITED(status2) != 0){
            printf("procces two done with status = %d\n", WEXITSTATUS(status2));
        }
        else{
            perror("something wrong in proccess two\n");
            return 0;
        }
        return 0;
    }else if (atoi(argv[5]) == 2){
        pid_t pid1 = fork();
        if (pid1 == -1){
            perror("can't create process one\n");
            return 0;
        }
        if (pid1 == 0){
            char* parametrs[3] = {argv[1], argv[2], NULL};
            execv(argv[1], parametrs);
            perror("you have problem with file1 or arg1\n");
            exit(-1);
        }
        int status1;
        waitpid(pid1, &status1, 0);
        if (WIFEXITED(status1) != 0){
            printf("procces one done with status = %d\n", WEXITSTATUS(status1));
        }
        else{
            perror("something wrong in proccess one\n");
            return 0;
        }

        pid1 = fork();
        if (pid1 == -1){
            perror("can't create process two\n");
            return 0;
        }
        if (pid1 == 0){
            char* parametrs[3] = {argv[3], argv[4], NULL};
            execv(argv[3], parametrs);
            perror("you have problem with file2 or arg2\n");
            exit(-1);
        }
        waitpid(pid1, &status1, 0);
        if (WIFEXITED(status1) != 0){
            printf("procces two done with status = %d\n", WEXITSTATUS(status1));
        }
        else{
            perror("something wrong in proccess two\n");
            return 0;
        }  
        return 0; 
    }
    perror("arvg[5] != 1 || 2\n");
    return 0;
}
