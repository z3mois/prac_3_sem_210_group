// Напишите функцию process, 
// обрабатывающую список за один проход от первого до последнего элемента 
// по нему следующим образом:
// void process(struct List *pl, const char *str);
// Звенья, у которых строка elem равна строке str удаляются,
//  а звенья, у которых строка elem лексикографически больше строки str
//   переставляются в конец списка. Прочие звенья не изменяются.
// При удалении звена необходимо освобождать память, занимаемую звеном 
// и строкой. В списке нет заглавного звена, и он не закольцован. 
// Поле elem, параметры pl и str никогда не равны NULL.
// На проверку сдается только функция process и необходимые для 
// нее директивы #include.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    struct Node *prev, *next;
    char *elem;
};
struct List
{
    struct Node *first, *last;
};

void process(struct List *pl, const char *str){
    struct Node* p= pl->first;
    struct Node *last = NULL;
    struct Node *first = NULL;	
	while (p!= NULL){
		if (strcmp(p->elem, str) == 0){
			if (pl->first == pl->last){ // udalenie
				free(p->elem);
				free(p);
				pl->first = NULL;
				pl->last = NULL;
				p = NULL;
			}
			else if (p == pl->last){
				free(p->elem);
				(p->prev)->next = NULL;
				pl->last = p->prev;
				free(p);
				p = NULL;
			}
			else if (p == pl->first){
				free(p->elem);
				p = p->next;
				pl->first = p;
				free(p->prev);
				p->prev = NULL;
			}
			else{
				(p->next)->prev = p->prev;
				free(p->elem);
				p = p->next;
				free((p->prev)->next);
				(p->prev)->next = p;
			}
		}
		else if (strcmp(p->elem, str) > 0){
			if (first == NULL)
				first = p;
			else if (first == p)
				break;// nashli
			if (pl->first == pl->last || p == pl->last){
				p = NULL;
			}
			else if (p->next != NULL){ 
				last = pl->last;
				last->next = p;
				if (p->prev != NULL)
					(p->prev)->next = p->next;
				else
					pl->first = p->next;
				(p->next)->prev = p->prev;
				p->prev = last;
				p = p->next;
				(last->next)->next = NULL;
				pl->last = last->next;
			}
		}
		else{
			p = p->next;
		}
	}
}