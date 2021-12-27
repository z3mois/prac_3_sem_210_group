#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>


int main(int argc, char** argv){
    FILE* f = fopen(argv[1], "r");
    int out = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC);
    int curr;
    while(fscanf(f, "%d", &curr) == 1){
        write(out, &curr, 4);
    }
    fclose(f);
    close(out);
    return 0;
}
