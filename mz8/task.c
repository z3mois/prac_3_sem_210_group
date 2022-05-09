#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/stat.h>
// вывод father son father son 3секунды, после с интервалом в 1 секунду
// только father через 1 сек
#define MAXLEN 1024

char flag = 0;
 
void child(int s) {
    kill(getpid(), SIGCHLD);
    printf("\nSons end\n");
    exit(0);
}
 
void father(int s) {
    printf("\nFathers end\n");
    exit(0);
}
 
void usr(int s) {
    return;
}
void hand(int s) {
    flag = 1;
}
 
 
int main (int argc, char** argv) {
    pid_t pid, parent = getpid();
    signal(SIGUSR1, usr);
    if ((pid = fork()) == -1) {
       perror("fork\n");
        exit(1);
    } else if (pid == 0) {
        signal(SIGALRM, child);
        alarm(3);
        while (1) {
            pause();
            printf("Son\n");
            kill(parent, SIGUSR1);
        }
    } else {
        signal(SIGCHLD, hand);
        signal(SIGINT, father);
        while (!flag) {
            printf("Father\n");
            kill(pid, SIGUSR1);
            pause();
        }
        while (1) {
            printf("Father\n");
            sleep(1);
        }
        wait(NULL);
        exit(0);
    }
    return 0;
}
