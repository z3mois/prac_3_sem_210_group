#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#define SSYMBS1 "()|&;><\0"
#define SSYMBS2 "|&>\0"


char my_shell(char** list);


//_______my_cd____________________


char my_cd(char* dir){      // текущая директория
    char temp;
    if ((temp = chdir(dir)) != 0)
        fprintf(stderr, "ERROR: no such directory %s\n", dir);
    return temp;
}
//_____________________________


//_______________entered___________________________________________


char ** entered(){
	// то что ввел пользователь пишем в q
    char *q = (char *)malloc(8 * sizeof(char));               // создаем строковый массив 
    if(q == NULL){
        fprintf(stderr, "ERROR: malloc error occurred\n");
        return NULL;
    }
	int buffer = 8;
	int k = 0;											//счетчик
	char c;                                            //посимвольно читаем
	while ((c = getchar()) != '\n'){
		q[k++] = c;
		if (--buffer == 0){
			q = (char *)realloc(q, k + 8);
			if(q == NULL){
                fprintf(stderr, "ERROR: realloc error occurred\n");
                return NULL;
            }
			buffer += 4;
		}
	}
	if(k == 0){											// ничего не ввели - ничего нет
	    free(q);
	    return NULL;
	}
	q[k] = 0;	
	//записываем строку с правильным количеством пробелов в p
	char* p = (char *)malloc(2 * strlen(q)  * sizeof(char));
	if(p == NULL){
        fprintf(stderr, "ERROR: malloc error occurred\n");
        return NULL;
    }
	k = 0;
	for(int i = 0; i < strlen(q); i++){
	    if((strchr(SSYMBS2, q[i]) != NULL) && (q[i] == q[i+1])){
	        p[k++] = ' ';
	        p[k++] = q[i++];
	        p[k++] = q[i];
	        p[k++] = ' ';
	    }
	    else if(strchr(SSYMBS1, q[i]) != NULL){
	        p[k++] = ' ';
	        p[k++] = q[i];
	        p[k++] = ' ';
        }
        else
            p[k++] = q[i];
	}
	p[k] = 0;
	free(q);
	// entered line
	char** user_str = (char **)malloc(8 * sizeof(char*));
	if(user_str == NULL){
        printf("ERROR: malloc error occurred\n");
        free(p);
        return NULL;
    }
	buffer = 4;
	k = 0;
	q = strtok(p, " \0");               // разбиваем строку на части  
	do{
	    user_str[k] = (char*)malloc(strlen(q) * sizeof(char));
	    strcpy(user_str[k++], q);
	    if (--buffer == 0){
			user_str = (char **)realloc(user_str, (k + 8) * sizeof(char*));
			buffer += 4;
		}
	    q = strtok(NULL, " \0");			// продолжаем делить строку
	}while(q != NULL);
	user_str[k] = q;                  // каждый элемент строки содержит указатель на указатель команды
	free(p);
	free(q);
	return user_str;
}
//_______________________________________________________________


//_____________Redirection_____________
// перенаправление ввода вывода

char Redirection(char** redirection){            //   работаем с файлом
						   //  > записывает в файл < читает из файла
    int desc;
    int i = 0;
    while(redirection[i] != NULL){
        if(strcmp(redirection[i], ">") == 0){
            desc = open(redirection[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if(desc == -1){
                fprintf(stderr, "ERROR: cannot open/create %s\n", redirection[i+1]);
                return 1;
            }
            dup2(desc, 1);
            close(desc);
        }
        else if(strcmp(redirection[i], "<") == 0){
            desc = open(redirection[i+1], O_RDONLY);
            if(desc == -1){
                fprintf(stderr, "ERROR: cannot open/create %s\n", redirection[i+1]);
                return 1;
            }
            dup2(desc, 0);
            close(desc);
        }
        i += 2;
    }
    return 0;
}
//______________________________________________


//______________command________________________
// реализуем конвейер 

char command(char** list, char fl){
    if(strcmp(list[0], "cd") == 0){
        if((list[1] == NULL) || (list[2] != NULL)){
            fprintf(stderr, "ERROR: wrong amount of arguments in cd\n");
            return 1;
        }
        else
            return my_cd(list[1]);  
    }
    if(strcmp(list[0], "(") == 0){
        int k = 0;
        while(list[++k] != NULL);
        list[k-1] = NULL;
        return my_shell(list+1);
    }
    char** redirection = (char**)malloc(sizeof(char*) * 5);
    if(redirection == NULL){
        fprintf(stderr, "ERROR: malloc error occurred\n");
        return 1;
    }
    int i = 0, j = 0;
    int siz = 0;
    while(list[siz++] != NULL);
    char** con = (char**)malloc(sizeof(char*) * siz);				// конвейер
    if(con == NULL){
        fprintf(stderr, "ERROR: malloc error occurred\n");
        free(redirection);
        return 1;
    }
    if((strcmp("<", list[0]) == 0) || (strcmp(">", list[0]) == 0)){
        redirection[j++] = list[0];
        redirection[j++] = list[1];
        i += 2;
        if((strcmp("<", list[2]) == 0) || (strcmp(">", list[2]) == 0)){
            redirection[j++] = list[2];
            redirection[j++] = list[3];
            i+= 2;
        }
    }
    else if((siz > 2) && ((strcmp("<", list[siz-3]) == 0) || (strcmp(">", list[siz-3]) == 0) )){
        redirection[j++] = list[siz-3];
        redirection[j++] = list[siz-2];
        siz -= 2;
        if((siz > 2) && ((strcmp("<", list[siz-3]) == 0) || (strcmp(">", list[siz-3]) == 0) )){
            redirection[j++] = list[siz-3];
            redirection[j++] = list[siz-2];
            siz -= 2;
        }
    }
    redirection[j] = NULL;
    j = 0;
    int con_size = 1;
    for(i = i; i<siz-1; i++){
        if(strcmp(list[i], "|") == 0)
            con_size++;
        con[j++] = list[i];
    }
    con[j] = NULL;
    char** temp = con;
    pid_t pid_array[con_size];
    int fd[2];
    for(i = 0; i < con_size; i++){
        pipe(fd);
        int prevfd;
        char** cur_command = (char**)malloc(sizeof(char*) * (siz));
        if(redirection == NULL){
            fprintf(stderr, "ERROR: malloc error occurred\n");
            free(redirection);
            free(con);
            return 1;
        }
        int k = 0;
        while(temp[k] != NULL && (strcmp(temp[k], "|") != 0)){
            cur_command[k] = temp[k];
            k++;
        }
        cur_command[k] = NULL;
        temp += k+1;
        if((pid_array[i] = fork()) == 0){      // сын
            if(!fl)
                signal(SIGINT, SIG_DFL);
            if(Redirection(redirection) == 1){       // делаем действия с файлом
                fprintf(stderr, "ERROR: redirection error\n");
                exit(1);
            }
            if(i != 0)
                dup2(prevfd, 0);
            if(i != con_size-1)
                dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execvp(cur_command[0], cur_command);
            fprintf(stderr, "ERROR: cannot execute %s\n", cur_command[0]);
            exit(1);
        }
        free(cur_command);
        prevfd = fd[0];
        close(fd[1]);
    }
    free(con);
    free(redirection);
    for(i = 0; i < con_size-1; i++)
        waitpid(pid_array[i], NULL, 0);
    int conv_st;
    waitpid(pid_array[con_size-1], &conv_st, 0);
    if(WIFEXITED(conv_st) == 0){                
        if(WSTOPSIG(conv_st) == SIGINT);      //  возвращает номер сигнала, из-за которого сын был остановлен
            return SIGINT;
        fprintf(stderr, "ERROR: conveyor executing error\n");
        return 1;
    }
    else
        return WEXITSTATUS(conv_st);
}
//________________________________________________


//___________condition_command______________________
// отправляем текущую команду на выполнение

char condition_command(char** list, char fl){
    char exit_st = 0;
    char condition = 0;
    char** cur;
    int k = 0;
    int i, ok;
    while(1){
        cur = (char**)malloc(256 * sizeof(char*));
        if(cur == NULL){
            fprintf(stderr, "ERROR: malloc error occurred\n");
            return 1;
        }
        i = 0;
        ok = 0;
        if(strcmp(list[k], "(") == 0){
            ok++;
            cur[i++] = list[k++];
            while(1){
                cur[i++] = list[k++];
                if(strcmp(list[k-1], "(") == 0)
                    ok++;
                else if(strcmp(list[k-1], ")") == 0)
                    ok--;
                if(!ok)
                    break;
            }
        }
        else
            while((list[k] != NULL) && (strcmp(list[k], "&&") != 0) && (strcmp(list[k], "||") != 0))
                cur[i++] = list[k++];
        cur[i] = NULL;
        if(exit_st == condition)              // если все успешно, то выполняем команду
            exit_st = command(cur, fl);               
        if(list[k] == NULL)
            break;
        condition = strcmp("&&", list[k]) == 0 ? 0 : 1;
        k++;
        free(cur);
    }
    free(cur);
    return exit_st;
}
//_________________________________________________


//_____________background_mode_________________________
// фоновый режим

char background_mode(char** list){
    pid_t pid;
    if((pid = fork()) == 0){    //  cын
        if(fork() == 0){        // внук
            int dev_null = open("/dev/null", O_RDWR);   // перенаправление 
            dup2(dev_null, 0);
            dup2(dev_null, 1);
            close(dev_null);
            signal(SIGINT, SIG_IGN);
            condition_command(list, 1);   //   выполнение внука
            sleep (20);
            exit(0);
        }
        exit(0);						// конец сына
    }
    int st;
    waitpid(pid, &st, 0);
    if(WIFEXITED(st) == 0)      //  завершился полохо 
        return 1;
    else
        return WEXITSTATUS(st);
}
//___________________________________________


//_____________my_shell__________________________
// отправляем команды либо в фоновый режим, либо на обычное выполнение, ожидаем пока завершится выполнение сына,
// возвращаем статус выполнения 

char my_shell(char** list){
    pid_t pid;
    if((pid = fork()) == 0){   //  сын 
        int k = 0;
        int i, j;
        char exit_st = 0;
        int bracket;
        while(1){
            char** current_list = (char**)malloc(256 * sizeof(char*));
            i = k;
            j = 0;
            bracket = 0;
            if(strcmp(list[i], "(") == 0){
                bracket++;
                current_list[j++] = list[i++];
                while(1){
                    current_list[j++] = list[i++];
                    if(strcmp(list[i-1], "(") == 0)
                        bracket++;
                    else if(strcmp(list[i-1], ")") == 0)
                        bracket--;
                    if(!bracket)
                        break;
                }
            }
            else{
                while((list[i] != NULL && (strcmp(list[i], "&") != 0) && (strcmp(list[i], ";") != 0)) || (bracket != 0)){
                    if(strcmp(list[i], "(") == 0){
                        bracket++;
                    }
                    if(strcmp(list[i], ")") == 0){
                        bracket--;
                    }
                    current_list[j++] = list[i++];
                }
            }
            current_list[j] = NULL;
            if((list[i] != NULL) && (strcmp(list[i], "&") == 0))   //  переход в фоновый режим
                exit_st |= background_mode(current_list);       
            else                                                  // переходим в состояние выполнения команды
                exit_st |= condition_command(current_list, 0);
            if(list[i++] != NULL)
                if(list[i] == NULL){
                    free(current_list);
                    break;
                }
                else
                    k = i;
            else{
                free(current_list);
                break;
            }
            free(current_list);
        }
        exit(exit_st);
    }
    int st;
    waitpid(pid, &st, 0);
    if(WIFEXITED(st) != 0)        // если сын завершился, возвращается 8 младших битов
        return WEXITSTATUS(st);
    else
        return -1;
}
//_________________________________________________________________


//__________del___________
// удаляем содержимое списка с освобождением памяти при завершении выполнения команды

void del(char** list){
    int k = 0;
    while(list[k++] != NULL)
        free(list[k-1]);
    free(list);
}
//__________________________


int main(){
   // signal(SIGINT, SIG_IGN);                  // не реагируем на CTRL + C
    char exit_st;
    char** user_str;
    while(1){
        printf(">");
        user_str = entered();					// вводим строку
        if(user_str != NULL){
            if(strcmp(user_str[0], "exit") == 0){    // если первая команды выход - освобождаем память и уходим
                del(user_str);
                printf("Good Bay!\n");
                break;
            }
            exit_st = my_shell(user_str);           //   отправляем команду на выполнение
            if(exit_st && exit_st != SIGINT)
                if(exit_st == -1)
                    fprintf(stderr, "ERROR: invalid command construction\n");
                else
                    fprintf(stderr, "ERROR: command exit status is %d\n", exit_st);
            del(user_str);         // освобождаем память перед завершнием команды 
        }
    }
    return 0;
}
