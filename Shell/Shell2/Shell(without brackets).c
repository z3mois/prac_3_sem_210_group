#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>

struct input{
    char** b;  //begin
    char** e;  //end
};
typedef struct input input;


char IsSpecialSymbol(char letter){
    char answer = 0;
    if((letter == '(') || (letter == ')') || (letter == ';') || (letter == '<')){
        answer = 1; 
    }
    if((letter == '>') || (letter == '&') || (letter == '|'))
        answer = 2;
    return answer;
}

input ReadStrk(){
    char strk[256];
    fgets(strk, 256, stdin);
    if(strk[0] == '\n'){ 
        input answer;
        answer.b = NULL;
        answer.e = NULL;
        return answer;
    }
    char space_strk[512]; 
    int i = 0;
    int j = 0;
    while((strk[i] != 0) && (strk[i] != '\n')){
        if(IsSpecialSymbol(strk[i])){
            space_strk[j++] = ' ';
            if((strk[i] == strk[i+1]) && (IsSpecialSymbol(strk[i]) == 2)){ 
                space_strk[j++] = strk[i++]; 
            }
            space_strk[j++] = strk[i++];
            space_strk[j++] = ' ';
        }
        else{
            space_strk[j++] = strk[i++];
        }
    }
    space_strk[j] = 0;
    char** strk_mass = (char**)malloc(sizeof(char*) * 100); 
    for(int k = 0; k < 100; k++){
        strk_mass[k] = (char*)malloc(sizeof(char) * 512);
    }
    int strk_number = 0;
    char flag = 0;  
    int letter_number = 0;
    i = 0;
    for(;;){
        if(space_strk[i] != ' '){
            flag = 1;
        }
        if(((space_strk[i] == ' ') && (flag == 1)) || (space_strk[i] == 0)){
            flag = 0;
            strk_mass[strk_number++][letter_number] = 0;
            letter_number = 0;
            if(space_strk[i] == 0){
                break;
            }
            i++;
        }
        else if(flag){
            strk_mass[strk_number][letter_number++] = space_strk[i++];
        }
        else
            i++;
    }
    strk_mass[strk_number][letter_number] = 0;
    
    input answer;
    answer.b = strk_mass; 
    answer.e = strk_mass + strk_number - 1; 
    if(answer.e[0][0] == 0) //пустая строка 
        answer.e--;
    return answer;
}

void PrintStrk(input inp){
    if(inp.b != NULL){
        char** cur = inp.b;
        while(cur != inp.e){
            printf("%s\n", cur[0]);
            cur++;
        }
        printf("%s\n", cur[0]);
    }
}

void Direction(input inp){
    
    if(inp.b != NULL){
        char* str = inp.b[0];
        int file;
        if(strcmp(str, ">") == 0){
            file = open(inp.b[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            dup2(file, 1);
            close(file);
        }
        else if(strcmp(str, ">>") == 0){
            file = open(inp.b[1], O_WRONLY | O_APPEND);
            if(file == -1){
                file = creat(inp.b[1], 0666);
            }
            dup2(file, 1);
            close(file);
        }
        else if(strcmp(str, "<") == 0){
            file = open(inp.b[1], O_RDONLY);
            dup2(file, 0);
            close(file);
        }

        if(inp.b+1 != inp.e){  //проверяем есть ли 3 и 4 строка 
            str = inp.b[2];
            if(strcmp(str, ">") == 0){
                file = open(inp.b[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
                dup2(file, 1);
                close(file);
            }
            else if(strcmp(str, ">>") == 0){
                file = open(inp.b[1], O_WRONLY | O_APPEND);
                if(file == -1){
                    file = creat(inp.b[3], 0666);
                }
                dup2(file, 1);
                close(file);
            }
            else if(strcmp(str, "<") == 0){
                file = open(inp.b[3], O_RDONLY);
                dup2(file, 0);
                close(file);
            }
        }
    }
}

char Conveyor(input inp){
    input redir;
    input conv;
    conv.b = inp.b;
    conv.e = inp.e;
    redir.b = NULL;
    redir.e = NULL;
    if(IsSpecialSymbol(inp.b[0][0])){
        redir.b = inp.b;
        if(IsSpecialSymbol(inp.b[2][0])){
            redir.e = inp.b+3;
        }
        else{
            redir.e = inp.b+1;
        }
        conv.b = redir.e+1;
    }
    char flag = 1;
    int i = 0;
    int prevfd;
    pid_t pid_mass[100];
    int fd[2];
    while(flag){
        char** curcom = (char**)malloc(sizeof(char*) * 100);
        int k = 0;
        while(flag && (strcmp(conv.b[0], "|") != 0)){
            curcom[k++] = conv.b[0];
            if(conv.b != conv.e)
                conv.b++;
            else
                flag = 0;
        }
        if(flag){
            conv.b++;
        }
        curcom[k] = NULL;
        pipe(fd);
        if((pid_mass[i] = fork()) == 0){
            Direction(redir);
            if(i!=0){
                dup2(prevfd, 0);
            }
            if(flag){
                dup2(fd[1], 1);
            }
            close(fd[0]);
            close(fd[1]);
            execvp(curcom[0], curcom);
            exit(1);
        }
        i++;
        free(curcom);
        prevfd = fd[0];
        close(fd[1]);
    }
    //i--;
    for(int j = 0; j < i-1; j++){
        waitpid(pid_mass[j], NULL, 0);
    }
    int conv_status;
    waitpid(pid_mass[i-1], &conv_status, 0);
    if(WIFEXITED(conv_status) == 0){
        return 1; //conveyor code ERROR
    }
    else{
        return WEXITSTATUS(conv_status);
    }
}



char IfCommand(input inp){

    char curop = 0;
    char curexit = 0;

    input curcom;
    for(;;){
        curcom.b = inp.b;
        curcom.e = inp.b;
        while((curcom.e <= inp.e) && (strcmp(*curcom.e, "||") != 0) && (strcmp(*curcom.e, "&&") != 0)){
            curcom.e++;
        }
        curcom.e--;
        if(curop == curexit){
            curexit = Conveyor(curcom);
        }
        if(curcom.e != inp.e){
            if((curcom.e+1)[0][0] == '|')
                curop = 1;
            else
                curop = 0;
            inp.b = curcom.e+2;
        }
        else {
            break;
        }
    }

    return curexit;
}

char OffShore(input inp){
    pid_t pid;
    if((pid = fork()) == 0){
        if(fork() == 0){
            int dev_null = open("/dev/null", O_RDWR);
            dup2(dev_null, 0);
            dup2(dev_null, 1);
            close(dev_null);
            signal(SIGINT, SIG_IGN);
            IfCommand(inp);
            //sleep(10);
            exit(0);
        }
        exit(0);
    }
    int status;
    waitpid(pid, &status, 0);
    if(WIFEXITED(status) == 0){
        return 1;
    }
    else{
        return WEXITSTATUS(status) == 0 ? 0 : 1;
    }
}

void All(input inp){
    input curcom;
    for(;;){
        curcom.b = inp.b;
        curcom.e = inp.b;
        while((curcom.e <= inp.e) && (strcmp(*curcom.e, ";") != 0) && (strcmp(*curcom.e, "&") != 0)){
            curcom.e++;
        }
        curcom.e--;
        if((curcom.e == inp.e) || (curcom.e+1)[0][0] == ';'){
            IfCommand(curcom);
        }
        else{
            OffShore(curcom);
        }
        if(curcom.e < inp.e - 1){
            inp.b = curcom.e+2;
        }
        else{
            break;
        }
    }
}



int main(){
    input inp = ReadStrk();
    All(inp);
    //printf("%s %s\n", inp.b[0], inp.e[0]);
}
