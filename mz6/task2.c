#include<stdio.h>
#include<string.h>
// В аргументах командной строки передаются 9-битные восьмеричные числа, 
// соответствующие правам доступа к файлам в UNIX. 
// Каждый аргумент командной строки содержит только одно число. 
// Незначащие нули у восьмеричного числа в могут отсутствовать, а могут и присутствовать. 
// На стандартный поток вывода напечатайте права доступа в rwx-записи. 
// Каждую строку прав доступа выводите на отдельной строке текста.
// Для получения строки-результата используйте специально
// посвященную этому константную строку-шаблон rwxrwxrwx.
void prava(int x){
    if (x == 0){
        printf("---");
        return;
    }
    if (x == 1){
        printf("--x");
        return;
    }
    if (x == 2){
        printf("-w-");
        return;
    }
    if (x == 3){
        printf("-wx");
        return;
    }
    if (x == 4){
        printf("r--");
        return;
    }
    if (x == 5){
        printf("r-x");
        return;
    }
    if (x == 6){
        printf("rw-");
        return;
    }
    if (x == 7){
        printf("rwx");
        return;
    }
}
int main(int argc, char** argv){
    if (argc < 2){
        return 0;
    }
    int x = 0;
    for (int i = 1; i < argc; i++){
        if (strlen(argv[i]) == 3){
        x = argv[i][0] - '0';
        prava(x);
        x = argv[i][1] - '0';
        prava(x);
        x = argv[i][2] - '0';
        prava(x);  
        }
        else {
            if (strlen(argv[i]) == 2){
                prava(0);
                x = argv[i][0] - '0';
                prava(x);
                x = argv[i][1] - '0';
                prava(x);  
            }
            else if (strlen(argv[i]) == 1){
                    prava(0);
                    prava(0);
                    x = argv[i][0] - '0';
                    prava(x);  
            }
        }   
        printf("\n");   
    }
    return 0;
}