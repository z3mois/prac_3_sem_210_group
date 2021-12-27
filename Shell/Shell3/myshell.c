#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#define LEN 1024
/*
    & - запуск в фоновом режиме
    | - конвейр
    Второй процесс в связке с операцией || запускается только тогда, когда первый
    завершился с системным или пользовательским кодом завершения, не равным 0
    > перенаправляет вывод в файл, перезаписывая его
    >> перенаправляет вывод в файл, но не перезаписывает его, а добавляет в конец
*/
int check_spec_symb = 0;
int check_new_line = 0;
int check_eof = 0;
int check_background = 0;
int number = -1;

typedef struct save {
    char *str;
    int symbol;
    int numb;
    struct save *next;
} save;

void clear_lst (save* lst){
	if(lst != NULL){
        if (lst->next != NULL){
            clear_lst(lst->next);
        }
        if (lst->str != NULL){
            free(lst->str);
            lst->str = NULL;
        }
        free(lst);
        lst = NULL;
    }
}

void printf_lst (save *lst){
    while (lst){
        printf("%s ", lst->str);
        lst = lst->next;
    }
}

int check_symb (char c){       // if c - symb => 1 else 0
    if (c == '&' || c == '>' || c == '|' || c == '<'){
        return 1;
    }
    return 0;
}

char *read_words(void){
    check_spec_symb = 0;
    int check_command = 0;
    int check_spec = 0;
    check_new_line = 0;
    char c;
    int i = 0;
    char *str = malloc(sizeof(char));
    while ((c = getchar()) != EOF){
        if (check_spec){    // check for special symbols
            check_spec = 0;
            if (c == str[i-1]){
                str = realloc(str, (i+2)*sizeof(char));
                str[i] = c;
                str[i+1] = '\0'; 
                return str;
            } else {
                ungetc(c, stdin); // return to the stream
                str[i] = '\0';  
                return str; 
            }
        } else if (c == ' ' || c == '\n'){
            if (check_command){  // command formed
                if(c == '\n'){
                    check_new_line = 1;
                }
                str[i] = '\0';
                if (strcmp(str, "exit") == 0){
                    check_eof = 1;
                }
                return str;
            }else {     // command didn't formed
                if (c == '\n'){
                    check_new_line = 1;
                    free(str);
                    // WARN
                    return NULL;
                }
                continue; // skip space 
            }
        } else if (check_symb(c)){     // work with & < > |
            if (check_command){     // end word here
                ungetc(c, stdin);
                str[i] = '\0';
                return str;
            } else {
                check_spec = 1;
                check_spec_symb = 1;
                str = realloc(str, (i+2)*sizeof(char));
                str[i] = c;
                i ++;
            }
        } else if (c == ';' || c == ')' || c == '('){
            if (check_command){ // end word here
                ungetc(c, stdin);
                str[i] = '\0';
                return str;
            } else {
                str = realloc(str, sizeof(char) * (i+2));
                str[i] = c;
                i++;
                str[i] = '\0';
                check_spec_symb = 1;
                return str;
            }
        } else {
            check_command = 1; // start command
            str = realloc(str, (i+2)*(sizeof(char)));
            str[i] = c;
            i ++;
        }
    }
    check_eof = 1;
    // ??????????????????
    str[i] = '\0';
    return str;
}

save *add_list(save* lst, char *str){
    if (lst == NULL){
        save *help = malloc(sizeof(save));
        help->str = malloc((strlen(str) + 1) * sizeof(char));
        help->symbol = check_spec_symb;
        strcpy(help->str, str);
        help->next = NULL;
        help->numb = number;
        return help; 
    }
    lst->next = add_list(lst->next, str);
    return lst;
}

char **list_to_array(save *l) {
    char **res = NULL;
    int i = 0;
    while (l) {
        res = (char **)realloc(res, sizeof(char *) * (i + 2));
        res[i] = (char *)malloc(sizeof(char) * (strlen(l->str) + 1));
        strcpy(res[i], l->str);
        i++;
        l = l->next;
    }
    res[i] = NULL;
    return res;
}

save* change_list (save* lst, int cnt){
    if (lst == NULL){
        return lst;
    }
    if(cnt != 0){
        lst->next = change_list(lst->next, cnt - 1);
    } else {
        return NULL;
    }
    return lst;
}

save* change(save *lst, save* check){
    if (lst == NULL){
        return check;
    }
    lst->next = change(lst->next, check);
    return lst;
}

int conveyor(save *lst){
    int check_end = 0;
    int check_first = 0;
    pid_t last;
    save *start = lst;
    save *new = NULL;
    save *to_change = NULL;
    int prev[2];
    int cnt = 0;
    while (check_end == 0){
        to_change = start;
        cnt = 0;
        while (start != NULL){    // разделяем команды в конвейре
            if (strcmp(start -> str, "|") == 0){
                break;
            }
            cnt ++;
            start = start -> next;
        }
        if (start == NULL){
            check_end = 1;
        }
        if (start != NULL){
            start = start -> next;
        }
        new = change_list (to_change, cnt);
        int cur[2];
        if (check_end == 0){
            pipe(cur);
        }
        if ((last = fork()) == 0){
            if (check_first > 0){  // если не первыый, то объединяем потоки
                dup2(prev[0], 0); // ввод из prev
            }
            if (check_end == 0){
                close(cur[0]);
                dup2(cur[1], 1);  // вывод в cur
            } 
            // добавить
            char **res;
            res = list_to_array(new);
            execvp(new -> str, res); // ????
        }
        if (check_end == 0){ 
            close(cur[1]);
        }
        if (check_first > 0){ // закрываем канал
            close(prev[0]);  
        }
        prev[0] = cur[0];
        prev[1] = cur[1];
        check_first ++;
    }
    // возвращаем результаты завершений конвейра
    pid_t pid;
    int status;
    int stat_to_ret = 0;
    while((pid = wait(&status)) != -1){
        if (pid == last){
            stat_to_ret = WEXITSTATUS(status);
        }
    }
    return stat_to_ret;
}

int quotes (save *lst){ // обработка < > >>
    if (strcmp(lst->str, "<") == 0){
        int fd = open(lst->next->str, O_RDONLY);
        dup2(fd, 0);
        close(fd);
        return 1;
    } else if (strcmp(lst->str, ">") == 0){
        int fd = open(lst->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        dup2(fd,1);
        close(fd);
        return 1;
    } else if (strcmp(lst->str, ">>") == 0){
        int fd = open(lst->next->str, O_WRONLY | O_CREAT | O_APPEND, 0666);
        dup2(fd,1);
        close(fd);
        return 1;
    }
    return 0;
}

int proc (save* lst){ // work with >> > <
    if (fork() == 0){   // создаем сына для работы с командой
        save *start = lst;
        save *help = NULL;
        int tr;
        int cnt = 0;
        cnt = 0;
        if (quotes(lst)){ // если стоит в начале
            start = start -> next;
            start = start -> next;
            if (quotes(start)){
                start = start -> next;
                start = start -> next;
            }
            tr = conveyor(start);
            if (tr == 0){
                exit(1);
            } else {
                exit(0);
            }
        } else {
            while (start != NULL){
                if ((strcmp(start->str, ">>") == 0) || 
                (strcmp(start -> str, ">") == 0) ||
                (strcmp(start -> str, "<") == 0)){
                    if (quotes(start)){
                        start = start -> next;
                        start = start -> next;
                        if (start != NULL){
                            if (quotes(start)){
                                start = start -> next;
                                start = start -> next;
                            }
                        }
                    }
                    break;
                }
                cnt ++;
                start = start -> next;
            }
            help = change_list(lst, cnt);
            tr = conveyor(help);
            if (tr == 0){
                exit(1);
            } else {
                exit(0);
            }
        }
    }
    int st;
    wait(&st);
    return st;
}

int new_stage(save *lst){ // обработка && и ||
    int check_end = 0;
    int cnt = 0;
    int check1 = 0;
    int check2 = 0;
    save* start = lst;
    save* help = NULL;
    while (start != NULL){
        if (strcmp(start->str, "&&") == 0){
            check1 = 1;
            break;
        } else if (strcmp(start->str, "||") == 0){
            check2 = 1;
            break;
        }
        start = start->next;
        cnt++;
    }
    if (start == NULL){
        check_end = 1;
    }
    help = change_list(lst, cnt);
    int back;
    back = proc(help);
    if (check_end == 0){
        if ((back && check1) || (!back && check2)){
            back = new_stage(start->next);
        }
    }
    return back;
}

int work (save *lst){
    save* start = lst;
    save* start2 = NULL;
    save* check = NULL;
    save *help = NULL;
    save *help2 = NULL;
    save *help3 = NULL;
    int cnt = 0;
    int check_end = 0;
    int check_br = 0;
    while (!check_end){ // для обработки скобок
        char *prev = NULL;
        while (!check_end){
            if (start == NULL){
                check_end = 1;
                break;
            }
            if ((strcmp(start->str, "(") == 0) && (prev == NULL ||
            strcmp(prev, ";") == 0 || strcmp(prev, "&") == 0 )){
                check_br = 1;
                start = start->next;
                break;
            }   
            prev = start->str;
            start = start->next;
            cnt++;
        }
        if (check_end){
            break;
        } else{
            help3 = change_list(lst, cnt);
            
        }
        if (start != NULL){
            check = start;
            int is_open = 1;
            cnt = 0;
            while ((strcmp(check->str, ")") != 0) || (is_open != 1)){
                if ((strcmp(check->str, "(") == 0)){
                    is_open ++;
                }
                if ((strcmp(check->str, ")") == 0)){
                    is_open --;
                }
                cnt ++;
                check = check->next;
            }
            if (check != NULL){
                check = check->next;
            }
            help3 = change(help3, check);
            help = change_list(start, cnt);
            if (fork() == 0){ // создаю новый процесс который работает 
            //в скобках
                work(help);
                exit(0);
            }
            wait(NULL);
            work(help3);
            return 0;
        }
    } // заканчивается обработка скобок
    if (check_br == 1){
        start2 = help3;
    } else {
        start2 = lst;
    }
    if (start2 != NULL){
        if ((strcmp(start2 -> str, "&") != 0) && (strcmp(start2 -> str, ";") != 0)) {           
            check_end = 0;
            int check_2 = 0;
            cnt = 0;
            while(start2 != NULL){
                if (strcmp(start2->str, "&") == 0){
                    check_2 = 1;
                    break;
                }
                if (strcmp(start2->str, ";") == 0){
                    break;
                } 
                start2 = start2->next;
                cnt++;
            }
            if (start2 == NULL){
                check_end = 1;
            } else if (start2->next == NULL){ 
                check_end = 1;
            }
            help2 = change_list(lst, cnt); // обрезаю спсиок
            if (check_2){ // фоновый режим
                if (fork() == 0){
                    if (fork() == 0){
                        int fd[2];
                        signal(SIGINT, SIG_IGN);
                        fd[0] = open ("/dev/null", O_RDONLY);
                        fd[1] = open ("/dev/null", O_WRONLY);
                        dup2 (fd[0], 0);
                        dup2 (fd[1], 1);
                        new_stage(help2);
                        return 0;
                        exit(0);
                    } else {
                        exit(0);
                    } 
                    exit(0);
                } else {
                    wait(NULL);
                } 
            } else {
                new_stage(help2);
            } 
            if (check_end == 0){
                work(start2->next);
            }
        } else {
            work(start2->next);
        }
    }
    return 0; // success
}

int main(int argc, char **argv){
    char *word = NULL; // считываем сюда слова
    int count;
    while (check_eof == 0){
        check_new_line = 0;
        check_spec_symb = 0;
        save *list = NULL;
        while((check_eof == 0) && (check_new_line == 0)){
            word = read_words();
            number ++;
            if (word){
                list = add_list(list, word); // заполняем строку
                free(word);
            }
            word = NULL;
            count++;
        } // сохрнили строку в списке
        number = 0;
        work(list);
        clear_lst(list);
    }
    return 0;
}
