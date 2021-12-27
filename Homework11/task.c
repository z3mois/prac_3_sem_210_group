#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/stat.h>
#define max_size 1024
/*
Программа. Процесс в командной строке получает имя файла. 
Длина строк в файле ограничена. 
Создает 2 сыновей. 
Сыновья читают по строке из файла и выводят их на печать
строго поочередно. 1-ый выводит нечетные строки, а 2-ой – четные, 
нумерация с 1. Каждый перед своей строкой выводит также свой pid.
Синхронизировать работу сыновей через отца с помощью сигналов.
Завершение работы — аккуратное, убивать никого не надо.
*/
int read_string(int f, char* str){
    char c = 0;
    int i = 0;
    while (read(f, &c, 1)){
        if (c == '\n'){
            break;
        }
        str[i++] = c;
    }   
    str[i] = '\0';
    if (i == 0 && c != '\n') {
        return 0;
    }
    return 1;
}
static int counter = 0;
static int ok = 1;
void f1(int s){
}
void fff(int s){
}
void end(int s){
    ok = 0;
}
int main(int argc, char** argv){
    counter = 0;
    if (argc != 2){
        perror("error with arguments\n");
        return 0;
    }
    int f = open(argv[1], O_RDONLY, 0666);
    if (f == -1){
        perror("wrong with answer\n");
        return 0;
    }
    pid_t pid1, pid2;
    if((pid1 = fork()) < 0){
		printf("Fork error\n");
		close(f);
		exit(1);
	}
    if (pid1 == 0){
        signal(SIGUSR1, f1);
        char string[max_size];
        //printf("pid1 = %d\n",getppid());
        while(1){
            //printf("do1\n");
            pause();
            //printf("posle1\n");
            if (read_string(f, string)){
                printf("pid1 = %d : %s\n", getpid(), string);
                kill(getppid(), SIGUSR2);
            }
            else {
                kill(getppid(), SIGINT);
                exit(0);
            }
        }
        kill(getppid(), SIGINT);
        exit(0);
    }
    if((pid2 = fork()) < 0){
		printf("Fork error\n");
		close(f);
		exit(1);
	}
    if (pid2 == 0){
            signal(SIGUSR1, f1);
            char string[max_size];
            //printf("pid2 = %d\n",getpid());
            while(1){
                //printf("do2\n");
                pause();
                //printf("posle2\n");
                if (read_string(f, string)){
                    printf("pid2 = %d : %s\n", getpid(), string);
                    kill(getppid(), SIGUSR2);
                }
                else {
                    kill(getppid(), SIGINT);
                    exit(0);
                    //printf("else2\n");
                }
            }
            kill(getppid(), SIGINT);
            exit(0);
    }else {
            signal(SIGUSR2, fff);
            signal(SIGINT, end);
            while(ok){
                //printf("start\n");
                counter++;
                //printf("i = %d\n", counter);
                if (counter % 2 == 0){
                    //printf("%d", counter);
                    kill(pid2, SIGUSR1);
                }
                else {
                    //printf("i = %d\n", counter);
                    kill(pid1, SIGUSR1);
                }
                //printf("pered\n");
                pause(); 
                //printf("posle\n");
            }
            kill(pid1, SIGUSR1);
            kill(pid2, SIGUSR1);
            wait(NULL);
            wait(NULL);
    }
    close(f);
    return 0;
}
