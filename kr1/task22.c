#include<stdio.h>

#define BINPRINT(x) \
	do {\
        unsigned long long one = 1; \
        one <<= sizeof(x) * 8 - 1;\
        for(int i = 0; i < sizeof(x) * 8; ++i) {\
            printf("%d", (x & one) ? 1 : 0);\
            one >>= 1;\
        }\
        printf("\n");\
	}while(0 == 1);\

int invert(int x, int pos) {
    int mask = 0;
    int i = 0;
    while (i < pos) {
        i++;
        mask = mask << 1;
        mask = mask|1;
    }
    int k = x&mask;
    mask = ~mask;
    x = x&mask;
    x = ~x;
    x = x|k;
    return x;
}

int main(){
    int x = 32141;
    BINPRINT(x);
    x = invert(x, 4);
    BINPRINT(x);
    return 0;
}