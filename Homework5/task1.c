#include<stdio.h>
#include <stdint.h>
/*
1. Написать макрос, который выводит на экран в двоичном виде целое число
 (должен работать с целыми от char до long long). 
 Оформить в виде макроса с одним параметром — число, 
 которое нужно вывести. Как основу используйте функцию уже написанную в дз1.
В main использовать макрос для вывода данных разных типов.
Должен работать, например, такой фрагмент:
char c=-3;
if(c) BINPRINT(c); else printf("Zero!\n");
BINPRINT((short)7);
long x=15;
if(x) BINPRINT(x); else printf("Zero!\n");
BINPRINT((long long)x);
*/

#define BINPRINT(x) \
	do {\
        uint64_t one = 1; \
        one <<= sizeof(x) * 8 - 1;\
        for(int i = 0; i < sizeof(x) * 8; ++i) {\
            printf("%d", (x & one) ? 1 : 0);\
            one >>= 1;\
        }\
        printf("\n");\
	}while(0 == 1);

int main() {
    char c = -3;
    if(c)
        BINPRINT(c)
    else
        printf("Zero!\n");
    BINPRINT((short)7);
    long x = 15;
    if(x)
        BINPRINT(x)
    else
        printf("Zero!\n");
    BINPRINT((long long)x);
    return 0;
}
