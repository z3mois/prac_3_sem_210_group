#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*
Написать функцию, которая по списку слов строит бинарное дерево поиска.
Список слов берем из ДЗ-5.
Написать функцию вывода на экран данных из дерева.
Написать функцию удаления дерева.
*/
typedef struct Node{
    char* value;
    struct Node* next;
} Node;

typedef struct tree {
    char* value;
    struct tree* right;
    struct tree* left;
} tree;

typedef struct word{
    char value;
    struct word* next;
}word;

int check; // �� ���� ��� �������� int � ������� ��� �������� ������ ����� �� ��� ���������
word* readword(){
    word* answer = NULL;
    char c;
    scanf("%c", &c);
    word *p = answer;
    while (c  == ' ') {
        scanf("%c", &c);
    }
    if (c == EOF || c == '\n'){
        check = 0;
        return answer;
    }
    while (c != ' ' && c!= '\n' && c != EOF){
        word *tmp = (word*)malloc(sizeof(word));
        tmp->value = c;
        tmp->next = NULL;
        p = answer;
        if (answer != NULL) {
            while (p->next != NULL){
                p = p->next;
            }
            p->next = tmp;
        }
        else {
            answer = tmp;
        }

        scanf("%c", &c);
    }
    if (c != EOF && c != '\n'){
        check = 1;
        return answer;
    }
    else {
        check = 0;
        return answer;
    }
}

char* Word_to_str(word* wordd){
    word* len = wordd;
    int length = 0;
    while (len != NULL) {
        length++;
        len = len->next;
    }
    //printf("%d\n" , length);
    char* answer = (char *) malloc(length + 1);
    int i = 0;
    word* tmp;
    while (wordd != NULL){
        answer[i] = wordd->value;
        tmp = wordd;
        wordd = wordd->next;
        free(tmp);
        i++;
    }
    answer[i] = '\0';
    //delete_word(wordd);
    return answer;
}
Node* Node_inp() {
    //printf("dassdada\n");
    Node* answer = NULL;
    Node* p = answer;
    char* temp = Word_to_str(readword());
    while (check) {
        //printf("%d ", check);
        Node* tmp = (Node*) malloc(sizeof(Node));
        if (temp[0] != '\0') {
            tmp->value = temp;
        //printf("%s ", temp);
            tmp->next = NULL;
            p = answer;
            if (answer != NULL) {
                while (p->next != NULL){
                    p = p->next;
                }
                p->next = tmp;
            }
            else {
                answer = tmp;
            }
        }
        else{
            free(tmp);
            free(temp);
        }
        temp = Word_to_str(readword());
    }
    Node* tmp = (Node*) malloc(sizeof(Node)); // ���������� ���������� �����
        if (temp[0] != '\0') {
            tmp->value = temp;
        //printf("%s ", temp);
            tmp->next = NULL;
            p = answer;
            if (answer != NULL) {
                while (p->next != NULL){
                    p = p->next;
                }
                p->next = tmp;
            }
            else {
                answer = tmp;
            }
        }
        else {
            free(tmp);
            free(temp);
        }
    return answer;
}
void out(Node* node) {
    while (node != NULL){
    printf("%s ", node->value);
    node = node-> next;
    }
}

void Build_tree(char* str, tree** derevo){
    if (*derevo == NULL) {
        tree* new_tree = (tree*) malloc(sizeof(tree));
        new_tree->left = NULL;
        new_tree->right = NULL;
        new_tree->value = str;
        *derevo = new_tree;
    }
    else {
        if (strcmp(str, (*derevo)->value) >= 0){
            Build_tree(str, &((*derevo)->right));
        }
        else{
            Build_tree(str, &((*derevo)->left));
        }
    }
}
void deleter2(tree** derevo){
    if(*derevo != NULL){
    deleter2(&((*derevo)->right));
    deleter2(&((*derevo)->left));
    free(*derevo);
    *derevo = NULL;
    }
}
void print_tree(tree* derevo) {
    if (derevo!=NULL) { 
        print_tree(derevo->left); 
        printf("%s ", derevo->value); 
        print_tree(derevo->right); 
    }
}

void deleter(tree* derevo){
    if(derevo != NULL){
        deleter(derevo->left);
        deleter(derevo->right);
        //printf("%s ", derevo->value);
        //free(derevo->value);
        free(derevo);
    }
}
int main() {
    //word* wrd = readword();
    //printf("dakmda");
    //printf("%d", len(wrd));
    //char* str = Word_to_str(wrd);
    //printf("%s", str);
    printf("Enter node :");
    Node* node = Node_inp();
    Node* p = node;
    tree* drevo = NULL;
    while (p!= NULL) {
        Build_tree(p->value, &drevo);
        p = p->next;
    }
    printf("your tree :");
    print_tree(drevo);
    printf("\n");
    deleter2(&drevo);
    //deleter(drevo);
    
    // if (drevo == NULL){
    //     printf("Ok");
    // }
    // else {
    //     printf("neok");
    // }
    
	Node* for_delete;
	while(node != NULL){
		free(node->value);
		for_delete = node;
		node = node->next;
		free(for_delete);
    }
    
}
