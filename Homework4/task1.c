#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/*
Читать из стандартного ввода последовательность слов 
до конца строки '\n’ или до EOF. Слова разделяются пробелами 
(количество подряд идущих пробелов >=1), в начале и в конце 
строки пробелы тоже возможны, но их наличие не обязательно.
Длина слов неограниченна.
Пользоваться функцией realloc() (и функцией ввода из дз-3-1 )
 при вводе строки нельзя, отрабатываем работу со списками!
Для хранения слов использовать список с полем данных типа char* , 
однонаправленный или двунаправленный — на ваше усмотрение. 
Для формирования отдельных слов тоже использовать список 
(поле данных char или небольшой массив из char).
Вывести на экран введенные слова в обратном порядке 
(относительно порядка ввода) .
Вывести на экран введенные слова в алфавитном порядке.
Всю динамически выделенную память освободить.
*/
typedef struct Node{
    char* value;
    struct Node* next;
} Node;

typedef struct word{
    char value;
    struct word* next;
}word;
int check;
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
// void delete_word(word* w){
//     	word* for_delete;
// 	while(w != NULL){
// 		for_delete = w;
// 		w = w->next;
// 		free(for_delete);
//     }
// }
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
    return answer;
}
void Outwords(Node* node) {
    if (node == NULL) {
        return;
    }
    Outwords(node->next);
    printf("%s ", node->value);

}
void out(Node* node) {
    while (node != NULL){
    printf("%s ", node->value);
    node = node-> next;
    }
}
void deleter2(Node* list){
    if (list->next == NULL) {
        free(list->value);
        free(list);
    }
    else {
        Node* p = list;
        Node* for_delete;
        while (p != NULL){
            for_delete = p->next;
            //printf("%s\n", p->value);
            //free(p->value);
            free(p);
            p = for_delete;
        }
    }
}
Node* Node_to_Elem(Node* word__in_Node, int n){
	for (int i = 0; i < n; i++)
		word__in_Node = word__in_Node->next;
	return word__in_Node;
}
void Sort(Node* node){
	int size = 0;
	Node* sizes = node;
	while(sizes != NULL){
		size++;
		sizes = sizes->next;
	}
	char* temp;
	//printf("%d\n", size);
	//printf("%d\n", size);
	for (int i = 0; i < size - 1; i++){
        for (int j = i + 1; j < size; j++) {
            //printf("%s %d %s %d\n" ,Node_to_Elem(node, i)->value, i, Node_to_Elem(node, j)->value, j);
            if(strcmp(Node_to_Elem(node, i)->value, Node_to_Elem(node, j)->value) > 0){
                temp = Node_to_Elem(node, i)->value;
                Node_to_Elem(node, i )->value = Node_to_Elem(node, j)->value;
                Node_to_Elem(node, j)->value = temp;
            }
        }
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
    printf("Reversed node :");
    Outwords(node);
    printf("\n");
    Sort(node);
    printf("Sort node :");
    out(node);
    printf("\n");
	Node* for_delete;
	while(node != NULL){
		free(node->value);
		for_delete = node;
		node = node->next;
		free(for_delete);
    }
    //deleter2(node);
    return 0;
}
//asfd as fkas fsa kfjsad kfl jkfs
