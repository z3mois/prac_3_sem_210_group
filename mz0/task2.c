#include<stdio.h>

int main() {
    int xx[5];
    char x = 1;
    int xxx = 2;
    long int xxxx = 222;
    int x1 = sizeof(char);
    int x2 = sizeof(int);
    int x3 = sizeof(unsigned int);
    int x4 = sizeof(char *);
    int x5 = sizeof(int *);
    int x6 = sizeof(long int);
    int x7 = sizeof(long long int);
    int x8 = sizeof(xxx/0);

    printf("%d\n", x8);
    printf("%d", xxx);
    int x9 = sizeof(double);
    int x10 = sizeof(float);
    int x11 = sizeof(_Bool);
    int x12 = sizeof(xx);
    return 0;

}