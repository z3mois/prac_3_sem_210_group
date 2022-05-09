#include<stdio.h>

int main() {

    unsigned short  int a1 = 255; //младший байт 1, старший 0
    signed char b1;
    b1 = (signed char ) a1;

    unsigned char a2 = 255;
    signed char b2;
    b2 = (signed char ) a2;

    unsigned char a22 = 127;
    signed char b22;
    b22 = (signed char ) a22;

    unsigned char a23 = 128;
    signed char b23;
    b23 = (signed char ) a23;

     unsigned char a3 = 255;
    signed short int b3;
    b3 = (signed short int ) a3;

    printf("M>n : %d\n", b1); 
    printf("M=n : %d\n", b2); 
    printf("M=n : %d\n", b22); 
    printf("M=n : %d\n", b23); 
    printf("M<n : %d\n", b3);   
    return 0;
}