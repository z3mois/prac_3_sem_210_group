#include<stdio.h>
#include<stdlib.h>
#define block 4

typedef struct info_about_str{
    int len;
    int start;
} info_about_str;

int compare(const void* lhs, const void* rhs){
    return ((*(info_about_str*)lhs).len > (*(info_about_str*)rhs).len);
}

char* inputstr(FILE* f, int* check, int* len){
    int buffer = block;
    char* answer = (char*) malloc(block * sizeof(char));
    int count = 0;
    char c;
    while (fscanf(f, "%c", &c) != -1){
        if (c != '\n'){
            *(answer + count ) =c;
            count++;
            if ((buffer + count)%block == 0){
                buffer += block;
                answer = (char*) realloc(answer, buffer*sizeof(char));
            }
        }
        else {
            *(answer + count) = '\0';
            break;
        }
    }
    if (c != '\n')
    if (fscanf(f, "%c", &c) == -1){
        *check = 0;
    }
    *len = count;
    return answer;
}
int main(int argc, char** argv){
    if (argc != 3){
        perror("need 3 arguments\n");
        return 0;
    }
    FILE* inp = fopen(argv[1], "r");
    if (inp == NULL){
        perror("not open input file");
        return 0;
    }
    FILE* out = fopen(argv[2], "w");
    if (out == NULL){
        perror("not open output file");
        return 0;
    }   
    int count_string = 0;
    int length_string = 0;
    int check = 1;
    int start_str = 0;
    info_about_str* matrix_info = NULL;
    char* temp_str;
    while(check){
        temp_str = inputstr(inp, &check, &length_string);
        count_string++;
        matrix_info = (info_about_str*)realloc(matrix_info, sizeof(info_about_str) * count_string);
        matrix_info[count_string - 1].len = length_string;
        matrix_info[count_string - 1].start = start_str;
        start_str += length_string + 1;
        free(temp_str);
        //printf("%d %d\n",matrix_info[count_string - 1].len, , matrix_info[count_string - 1].start);
    }
    int n;
    printf("enter number str:");
    scanf("%d", &n);
    while (n != 0){
        if (n < 0){
            printf("error, n must be > 0\n");
        }
        else 
        if (n > count_string){
            printf("n > count_strings\n");
        }
        else {
            printf("string with number %d :", n);
            //printf("%d %d\n",matrix_info[n - 1].len, matrix_info[n - 1].start);
            fseek(inp, matrix_info[n - 1].start, SEEK_SET);
            temp_str = inputstr(inp, &check, &length_string);
            printf("%s\n", temp_str);
            free(temp_str);
        }
        printf("enter number str:");
        scanf("%d", &n); 
    }
    qsort(matrix_info, count_string, sizeof(info_about_str), compare);
    for (int i = 0; i < count_string; i++){
            fseek(inp, matrix_info[i].start, SEEK_SET);
            temp_str = inputstr(inp, &check, &length_string);
            fprintf(out, "%s\n", temp_str);
            free(temp_str);
    }
    fclose(inp);
    fclose(out);
    free(matrix_info);
    return 0;
}
