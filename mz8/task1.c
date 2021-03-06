#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<sys/stat.h>
// вывод father слово1 son слово2 father слово3 son слово4 3секунды, после с интервалом в 1 секунду
// так 3 секунды или до конца файла
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
int create_string(int fd){
    char temp = 'a';
    while (read(fd, &temp, 1) == 1 && temp != '\n' && temp != ' '){
        putchar(temp);
    }
    putchar('\n');
    if (temp != '\n' && temp != ' '){
        return 0;
    }
    else{
        
        return 1;
    }
}
void end(int s){

}
int main (int argc, char** argv) {
    if (argc != 2){
        perror("error with arguments\n");
        return 0;
    }
    int fd = open(argv[1], O_RDONLY, 0666);
    if (fd == -1){
        perror("error with file\n");
        return 0;
    }
    int ok = 1;
    pid_t pid, parent = getpid();
    signal(SIGUSR1, usr);
    if ((pid = fork()) == -1) {
       perror("fork\n");
        exit(1);
    } else if (pid == 0) {
        signal(SIGALRM, child);
        alarm(3);
        while (1 & ok) {
            pause();
            printf("Son\n");
            ok = create_string(fd);
            kill(parent, SIGUSR1);
        }
    } else {
        signal(SIGCHLD, hand);
        signal(SIGINT, father);
        while (!flag && ok) {
            printf("Father\n");
            ok = create_string(fd);
            kill(pid, SIGUSR1);
            pause();
        }
        while (1 & ok) {
            printf("Father\n");
            ok = create_string(fd);
            sleep(1);
        }
        close(fd);
        exit(0);
    }
    return 0;
}