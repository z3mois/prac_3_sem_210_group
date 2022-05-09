#include<stdio.h>

int main() {
    signed char x = 128;
    unsigned char xx = 128;
    char a = 128;
    if (x == a)
        printf("signed");
    if (xx == a) 
        printf("unsigned");
    return 0;
}
