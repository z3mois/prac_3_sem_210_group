#include<stdio.h>

int main(int argc, char** argv){
    for (int j = 1; j < argc; j++){
        int i = 0;
        int count = 0;
        while (argv[j][i] != '\0'){
            if (argv[j][i] >= '0' && argv[j][i] <= '9'){
                count ++;
            }
            i++;
        }
        i = 0;
        if (count > 0) {
        while(argv[j][i] != '\0') {
            printf("%c", argv[j][i]);
            i++;
        }
        printf("\n");
        }
    }
    return 0;
}
