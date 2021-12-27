#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#define n 64
#define m 16
#define model " "

volatile int mode = 0, file = 0, helpfd; 
volatile int comandmode[16]; 
volatile char namefiles[16][16];
volatile int priority[16];
volatile pid_t pid1[16];

char readfiles(char a, int num){
    int c;
    int i = 0;
    printf("number == %d\n", num);
    namefiles[num][15] = a;
    while ((c = fgetc(stdin)) != '\n' && c != EOF){
        if((c == ';') || (c == '&') || (c == '|') || (c == ' ')){
            namefiles[num][i] = '\0';
            return c;
        } else {
            namefiles[num][i] = c;
        }
        i++;
    }
    //printf("i == %d\n",i);
    namefiles[num][i] = '\0';
    return '!';
}

void changemode(int num){
    int i = 0;
    while(comandmode[i] != -1){
        i++;
    }
    comandmode[i - 1] = num;
    return;
}


void printmode(volatile int* mode){
    for(int i = 0; i < 16; i++){
        printf("%d :: %d\n", i, mode[i]);
    }
    return;
}

char** MACuB(char** mas, char* word, char c){
    int s = 0;
    if (word == NULL)
        return mas;
    //printf("F\n");
    int i = 0;
    //printf("%x", mas[0]);
    while (mas[i]){
        i++;
        s++;
    }
    s++;
    mas = realloc(mas, (s+1)*sizeof(char*));
    if (c == '&')
        comandmode[i] = 2;
    if (c == '|')
        comandmode[i] = 1;
    if (c == 'a')
        comandmode[i] = 0;
    if (c == ';')
        comandmode[i] = 0;
    if (c == '(')
        comandmode[i] = 100;
    comandmode[i + 1] = -1;
    mas[i] = word;
    mas[++i] = NULL;
    
    return mas;
}

char** readstr(void)
{
    char * str = NULL;
    int len = 0;                  
    int k = 0;                     
    int c;
    int ok;
    int check1 = 0, check2 = 0;            ///////////////for check on &&(2) and ||(1)
    char* finishstr; 
    ok = 0;
    int i = 0;
    mode = 0;
    comandmode[0] = -1;
    char **massivecomands;
    massivecomands = (char **)malloc(8*sizeof(char *));
    massivecomands[0] = NULL;
    for(int h = 0; h < 16; h++){
        comandmode[h] = -2;
        priority[h] = 0;
    }
    for(int k = 0; k < 16; k++)
        for(int h = 0; h < 16; h++){
            namefiles[k][h] = ' ';
        }
    
    while ((c = fgetc(stdin)) != '\n' && c != EOF){
        if (c == '>'){
            ok = 0;
            c = readfiles('>', i);
            if (c == '!'){
                if (k == 0 && ((c == EOF)||(c == '\n'))){
                    //
                    printf("Empty string\n");
                    //free(massivecomands);
                    return massivecomands;
                }
                finishstr = (char*) malloc((k + 1) * sizeof(char));
                strncpy(finishstr, str, k);
                free(str);
                if (ok == 0)
                    k--;
                //printf("Why\n");
                finishstr[k] = '\0';
                massivecomands = MACuB(massivecomands, finishstr, 'a');
                return massivecomands;
            }
        }
        if (c == '<'){
            ok = 0;
            c = readfiles('<', i);
            if (c == '!'){
                if (k == 0 && ((c == EOF)||(c == '\n'))){
                    //rintf("Empty string\n");
                    //free(massivecomands);
                    return massivecomands;
                }
                finishstr = (char*) malloc((k + 1) * sizeof(char));
                strncpy(finishstr, str, k);
                free(str);
                if (ok == 0)
                    k--;
                //printf("Why\n");
                finishstr[k] = '\0';
                massivecomands = MACuB(massivecomands, finishstr, 'a');
                return massivecomands;
            }
        }
        //printf("%c\n", c);
        if (k + 1 > len)  {             
            len += n;
            str = realloc(str, len);
        }
            switch(c) {  
                case ' ':
                    if (ok){
                        str[k++] = c;
                        ok = 0;
                        check1 = 0;
                        check2 = 0;
                    }  
                    break;
                case '|':
                    if (check1){
                        mode = 11;
                        changemode(11);
                        ok = 0;
                    } else {
                        mode = 1;
                        if (k == 0 && ((c == EOF)||(c == '\n'))) return NULL;
                        finishstr = (char*)malloc((k + 1) * sizeof(char));
                        strncpy(finishstr, str, k);
                        //printf("Free1\n");
                        free(str);
                        str = NULL;
                        //printf("Free2\n");
                        if (ok == 0)
                            k--;
                        finishstr[k] = '\0';
                        //puts(finishstr);
                        massivecomands = MACuB(massivecomands, finishstr, c);
                        len = 0;
                        i++;
                        k = 0;
                        ok = 0;
                        check1 = 1;
                        check2 = 0;
                    }
                    break;
                case ';':
                    check1 = 0;
                    check1 = 0;
                    if (k == 0 && ((c == EOF)||(c == '\n'))) return NULL;
                    finishstr = (char*)malloc((k + 1) * sizeof(char));
                    strncpy(finishstr, str, k);
                    free(str);
                    str = NULL;
                    if (ok == 0)
                        k--;
                    finishstr[k] = '\0';
                    massivecomands = MACuB(massivecomands, finishstr, c);
                    len = 0;
                    i++;
                    k = 0;
                    ok = 0;
                    break;
                case '&':
                    //printf("chack2 == %d\n", check2);
                    if (check2){
                        mode = 22;
                        changemode(22);
                        ok = 0;
                    } else {
                        mode = 2;
                        if (k == 0 && ((c == EOF)||(c == '\n'))) return NULL;
                        finishstr = (char*)malloc((k + 1) * sizeof(char));
                        strncpy(finishstr, str, k);
                        free(str);
                        str = NULL;
                        if (ok == 0)
                            k--;
                        finishstr[k] = '\0';
                        //puts(finishstr);
                        massivecomands = MACuB(massivecomands, finishstr, c);
                        len = 0;
                        i++;
                        k = 0;
                        ok = 0;
                        check2 = 1;
                        check1 = 0;
                    }
                    break;
                case '(':
                    priority[i] = 1;
                    break;
                case ')':
                    priority[i] = 2; 
                    break;
                default:
                    check1 = 0;
                    check1 = 0;
                    ok = 1;
                    str[k++] = c;
            }
    }
    if (k == 0 && ((c == EOF)||(c == '\n'))){
        //printf("Empty string\n");
        //free(massivecomands);
        return massivecomands;
    }
    finishstr = (char*) malloc((k + 1) * sizeof(char));
    strncpy(finishstr, str, k);
    free(str);
    if (ok == 0)
        k--;

    //printf("Why\n");

    finishstr[k] = '\0';

    massivecomands = MACuB(massivecomands, finishstr, 'a');

    return massivecomands;
}

char** stringparsing(char *str){
    char **massive = malloc(n * sizeof(char*));
    char *word;
    int S = n;
    int i = 0;
    word = strtok(str, model);
    while (word != NULL){
        massive[i] = word;
        i++;
        if (i > S){
            S += n;
            massive = realloc(massive, S*sizeof(char *));
        }
        word = strtok(NULL, model);
    }
    massive[i] = NULL;

    return massive;
}


int numberofc(char **mas){
    int i = 0;
    if (mas) { 
        while (mas[i] != NULL){
            i++;
        }
    }
    return i;
}

void printmas(char **mas){
    int i = 0;
    if (mas) { 
        while (mas[i] != NULL){
            puts(mas[i]);
            i++;
        }
        //printf("%d\n", i);
    }
    return;
}

void memorylost(char **mas){
    int i = 0;
    if (mas) {
        while (mas[i] != NULL){
            free(mas[i]);
            i++;
        }
        free(mas);
    }
    return;
}
//////////////////////////////////////////////////////////////////execution///////////////////////////////////////////////////////////////////////
int execution1(char** str, int n1, int n2){
    char comand[1024];
    char** arguments;
    int notexit = 1;
    int i = n1;
    int count;
        //printmode(priority);
        if (str){
            count = numberofc(str);
            //printf("count == %d\n", count);
            //printmas(str);
            while(i <= n2){
                arguments = stringparsing(str[i]);  //dividing the string into arguments
                strcpy(comand, arguments[0]); // my comand
                if (comand == NULL){
                    printf("Empty string\n");
                } else {
             /////////////////exit
                    if (!strcmp(comand, "exit")){
                        notexit = 0;
                        free(arguments);
                        break;
                    }
             ////////////////cd
                    if (!strcmp(comand, "cd")){
                        if (arguments[2] != NULL){
                            printf("bash: cd: too many arguments\n");
                        } else
                        if (chdir(arguments[1]) != 0){
                            perror("lsh");
                        }
                        free(arguments);
                        break;
                    }
             //////////////conv//////////////////////////////////////
                    char comand2[1024];

                    strcpy(comand2, "/bin/");
                    strcpy(comand, strcat(comand2, comand));
                    if (comandmode[i] == 2){
                        pid_t pid;
                        if((pid = fork()) == 0){
                            if(fork() == 0){
                                int dev_null = open("/dev/null", O_RDWR, 0666);
                                dup2(dev_null, 0);
                                //dup2(dev_null, 1);
                                close(dev_null);
                                signal(SIGINT, SIG_IGN);
                                //puts(comand);
                                i = conv(comand, arguments, i);
                                exit(0);
                            } 
                            exit(0);
                        }
                        int status;
                        waitpid(pid, &status, 0); 
                    } else {
                        int status;
                        i = conv(comand, arguments, i);
                        //waitpid(pid1[i], &status, 0);
                    }
                 ////////////////  
                }
                free(arguments);
                i++;
            }
            
        }

    return notexit;    
}
/////////////////////////////////////////////////////////conv/////////////////////////////////////////////////////////////////
int conv(char* comand, char** arguments, int i){
    int fd[2];
    pid_t pid;
    if (comandmode[i] == 1){
        pipe(fd);
        pid = pid1[i];
    } else 
        if (i > 0)
            if (comandmode[i - 1] == 1)
                pipe(fd);

    if((pid = fork()) == -1){
        perror("Error!!!");
    }
    if (pid){
    ///////////////////father/////////////////////
        int status;
        if (comandmode[i] != 1)
            waitpid(pid, &status, 0);
        //printf("WEXITSTATUS == %d\n", WEXITSTATUS(status));
        if ((comandmode[i] == 11) && !WEXITSTATUS(status)) {// ||| - true
            int j = 0;
            while (comandmode[i + j] == 11){
                j++;
            }
            //printf(" j = %d\n", j);
            i += j;
            //if (comandmode[i] == 11) {/// (a && b || ....)
            //    i++;
            //}

        } else {    
            if ((comandmode[i] == 22) && WEXITSTATUS(status)) {/// && - false;
                int j = 0;
                while (comandmode[i + j] == 22){
                    j++;
                }
                //printf(" j = %d\n", j);
                i += j;
            }    
        }           
    } else {
        if ((comandmode[i] == 1)){ //////////////////conv///////
            //printf("Fikys1\n");
            if (i > 0) {
                dup2(helpfd, 0);
            }
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
        } else { 
            if (i > 0)
                if (comandmode[i - 1] == 1){
                    //printf("Fikys2\n");
                    if (i > 0) {
                        dup2(helpfd, 0);
                    }
                    close(fd[0]);
                    close(fd[1]);
                }
        }

        if (namefiles[i][15] == '>'){
            int newfile;
            newfile = open(namefiles[i],  O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(newfile, 1);
        }
        if (namefiles[i][15] == '<'){
            int newfile;
            newfile = open(namefiles[i],  O_RDONLY, 0666);
            dup2(newfile, 0);
        }

            //puts(comand);
        if ((comandmode[i] == 1) | (comandmode[i] == 0) | (comandmode[i] == -1) | (comandmode[i] == 22) | (comandmode[i] == 11) | (comandmode[i] == -1) | (comandmode[i] == 2) || (comandmode[i] == 100))
            if (execv(comand, arguments) == -1){
                printf("Not such file in directory\n");
            }
        exit(1);
    }////son finished

    if (comandmode[i] == 1){
        helpfd = fd[0];
        close(fd[1]);
    } else {
        if (i > 0)
            if (comandmode[i - 1] == 1){
                close(fd[1]);
            }
    }//////////////////////////////////////////////////////////////////////
    return i;
}

/////////////////////////////////////////////////////////main/////////////////////////////////////////////////////////////////
int main(){
    char** str;
    char** str2;
    int notexit = 1;

    while(notexit){
        str = readstr();
        //printmode(priority);
        //printmode(comandmode);
        for(int j = 0; j < m; j ++){
            if (priority[j]){
                //printf("j1 == %d\n", j);
                int k = j;
                while(priority[k] != 2){
                    //printf("priority\n");
                    //puts(str[k]);
                    k++;
                }
                //printf("k1 == %d\n", k);
                notexit = execution1(str, j, k);

                for (int j = 0; j < m; j++){
                if (comandmode[j] == 1){
                    int status;
                    waitpid(pid1[j], &status, 0);
                }
        }

                j = k;
            }
        } 

        if (notexit)
        for(int j = 0; j < m; j ++){
            if (!priority[j] && (comandmode[j] != -2) && (comandmode[j] != -1)){
                //printf("j2 == %d\n", j);
                int k = j;
                while(!priority[k] && (comandmode[k] != -2) && (comandmode[k] != -1)){
                    //printf("priority2\n");
                    //puts(str[k]);
                    k++;
                }
                //printf("k2 == %d\n", k);
                notexit = execution1(str, j, k - 1);
                j = k;
            }
        } 
        for (int j = 0; j < m; j++){
            if (comandmode[j] == 1){
                int status;
                waitpid(pid1[j], &status, 0);
            }
        }
        //printf("Check\n");
        memorylost(str);
    }
    return 0;
}

/*
mode == 1 | conveer
mode == 0 ;
mode == 11 ||
mode == 22 &&
mode == 3 <
mode == 4 >
mode == 100 () - priority
PRIORITY:
1 - begining
2 - end
3 - exit normal
4 - exit with error
*/
