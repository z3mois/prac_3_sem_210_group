#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

int main(int argc, char** argv){// ./a.out count
    for(int i = 0; i < atoi(argv[1]); i++){
        sleep(1);
        printf("task %s have pid %d\n", argv[0], getpid());
    }
    return 0;
}