#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#define size 20
typedef struct List {
    char* value;
    struct List* next;
} List;
List* read_list(){
    List* answer = NULL;
    char* curr = (char*)malloc(size);
    List* p = answer;
    //printf("%d", sizeof(char*));
    scanf("%[^'\n']s", curr);
    //printf("%s\n", curr);
    while (strcmp(curr, "end") != 0) {
        List* tmp = (List*)malloc(sizeof(List));
        tmp->value = curr;
        //printf("%d\n", tmp->value);
        tmp->next = NULL;
        p = answer;
        if (answer != NULL) {
            while (p->next != NULL){
                p = p->next;
            }
            p->next = tmp;
            //printf("%s\n", answer->next->value);
        }
        else {
            answer = tmp;
        }
        curr = (char*)malloc(size);
        scanf("%s", curr);
        //printf("%s", curr);
    }
    free(curr);
    return answer;
}
void out (List* list){
    while (list != NULL){
        printf("%s\n", list->value);
        list = list->next;
    }
}
void insert(List* list, char sym){
    List* p = list;
    //printf("%c", sym);
    while (p!= NULL){
        int len = strlen(p->value);
        //printf("%d ", len);
        //printf("%c ", p->value[len]);
        if (p->value[len - 1] == sym) {
            List* tmp = (List*)malloc(sizeof(List));
            char* temp = (char*)malloc(4);
            temp[0] = '2';
            temp[1] = '2';
            temp[2] = '2';
            temp[3] = '\0';
            tmp ->value = temp;
            tmp->next = p->next;
            p->next = tmp;
            p = p->next;
        }
        p = p->next;
    }
}
List* deleter(List* list, char sym){
    List* p = list;
    List* prev = p;
    //printf("%c", sym);
    while (p!= NULL){
        if (strchr(p->value, sym)!= NULL) {
            if (p == list){
                list = p->next;
                prev = p->next;
                free(p->value);
                free(p);
                p = prev;
            }
            else {
                prev->next = p->next;
                free(p->value);
                free(p);
                p = prev->next;
            }
        }
        else {
            prev = p;
            p = p->next;
        }
    }
    return list;
}
void deleter2(List* list){
    if (list->next == NULL) {
        free(list->value);
        free(list);
    }
    else {
        List* p = list;
        List* for_delete;
        while (p != NULL){
            for_delete = p->next;
            //printf("%s\n", p->value);
            //free(p->value);
            free(p);
            p = for_delete;
        }
    }
}
int main(){
    List* list = read_list();
    //out(list);
    //printf("/nS");
    char sym;
    scanf("%c", &sym);
    printf("***********\n");
    scanf("%c", &sym);
    insert(list, sym);
    printf("***********\n");
    out(list);
    printf("***********\n");
    scanf("%c", &sym);
    scanf("%c", &sym);
    // printf("%c\n", sym);
    list = deleter(list, sym);
    printf("***********\n");
    out(list);
    //deleter2(list);
	List* for_delete;
	while(list != NULL){
        free(list->value);
		for_delete = list;
		list = list->next;
		free(for_delete);
    }
    return 0;
}
