#include<stdio.h>
#include<stdlib.h>
#define bufferok 4
int count = 0;
char* ReadStr() {
    int buffer = bufferok;
    char *answer =(char*)malloc(buffer * sizeof(char));
    char temp;
    while(scanf("%c", &temp) != -1) {
        if (temp != '\n'){
            *(answer + count) = temp;
            count++;
            if ((buffer+count)%bufferok == 0) {
                buffer += bufferok;
                answer = (char*)realloc(answer, buffer * sizeof(char));
            }
        }
        else{
            *(answer + count) = '\0';
            break;
        }
    }
    return answer;
}
int check(char *temp, int i, int length) {
    //printf("falk %d %d", i - length, length);
    for (int j = i - length; j < i; j++){
        //printf("%c\n", temp[j]);
        if ((temp[j] >= 'a' && temp[j] <= 'z')||(temp[j] >= 'A' && temp[j] <= 'Z')){}
        else return 0;
    }
    return 1;
}
char *find(char *str){
    char *answer = malloc(count * sizeof(char) + 1);
    //char *temp = malloc (count * sizeof(char) + 1);
    int i = 0, j = 0, length = 0;
    int curr;
    //printf("%d", count);
    while (i <= count) {
        if (str[i] == ' ' || str[i] =='\0'){
            //temp[i] = str[i];
            i++;
            //printf("i =probel %d", i);
            if (check(str, i - 1, length) == 1) {
                for (int jj = 0; jj < length; jj++){
                    answer[j + jj] = str[jj + i - 1 - length];
                }
                j = j + length;
            }
            //printf("j = %d\n", j);
            //printf("%s\n", answer);
            length = 0;
        }
        else {
            //temp[i] = str[i];
            //printf("%s\n", temp);
            i++;
            length++;
        }
        //printf( "I =%d\n", i);
        
    }
    //free(temp);
    return answer;
}
int main() {
    char *string = ReadStr();
    //printf("%d", count);
    char *string1 = find(string);
    printf("%s\n", string1);
    free(string1);
    free(string);
    return 0;
}
