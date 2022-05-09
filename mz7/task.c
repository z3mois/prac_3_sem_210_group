#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include<sys/wait.h>

int main(int argc, char** argv){
    pid_t pid_process;
    int fd[2], fe[2];
    pipe(fd);
    pipe(fe);
    FILE* f;
    if ((f = fopen(argv[argc - 1], "w+")) == NULL){
        perror("can't open file\n");
        return 0;
    }
    if ((pid_process = fork()) < 0){
        perror("error with fork\n");
        return 0;
    }
    else if (pid_process == 0){
        char str[80] = {0}, c = 0;
        int count = 0, flag = 1;
        //close(fd[0]);
        //close(fd[1]);
        do {
            count = 0;
            read(fd[0], &c, sizeof(char));
            read(fd[0],str, sizeof(str));
            for(int i = 0; i < strlen(str); i++){
                if (str[i] == c){
                    count++;
                }
            }
            printf("count == %d\n", count);
            if(strcmp(str,"exit\n")){ 
                fprintf(f, "%d\n", count);
                fflush(f);
            }
            write(fe[1], &flag, sizeof(int));
        }while (strcmp(str, "exit\n") != 0);
        close(fd[0]);
        close(fe[0]);
        exit(-1);
    }
    else {
        char c, str[80];
        //close(fd[0]);
        int flag = 1, j = 0;
        do
        {   
            if (j > 0){
                read(fe[0], &flag, sizeof(int));
                printf("flags = %d\n",flag);
            }
            scanf("%c ",&c);
            fgets(str, 80, stdin);
            if(strcmp(str, "exit\n")) {
                fprintf(f, "%s", str);
                fflush(f);
            }
            write(fe[1], &flag, sizeof(int));
            write(fd[1], &c, 1);
            write(fd[1], str, sizeof(str));
            j++;
        }while(strcmp(str,"exit\n"));
        wait(NULL);
        close(fd[1]);
        close(fe[1]);
        fclose(f);
    }

    return 0;
}
