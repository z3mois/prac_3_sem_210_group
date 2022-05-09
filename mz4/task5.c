#include<stdio.h>
#include<limits.h>
// В аргументах командной строки задаются имена текстовых файлов,
//  содержащих последовательности целых чисел в порядке неубывания.
// На стандартный поток вывода напечатайте последовательность,
//  составленную из всех чисел во входных последовательностях в порядке 
//  их неубывания.
// Не используйте сортировку.
// Не забывайте завершать вывод программы символом \n.
// Каждый файл может быть открыт и прочитан не более одного раза.


// void input(long long *num_mas, FILE* mas_file[], int size, char** argv, int* how_many_open){
//     int i = 0;
//     while (i < size){
//         mas_file[i] = fopen (argv[i + 1], "r+");
//         if (mas_file[i] != NULL){
//             if (fscanf(mas_file[i], "%lld", &num_mas[i]) == -1){
//                 fclose(mas_file[i]);
//                 mas_file[i] = NULL;
//                 *how_many_open -= 1;
//             }
//         }
//         else {
//             *how_many_open -= 1;
//         }
//         i++;
//     }
// }
// void print(long long *num_mas, FILE* mas_file[], int size, char** argv, int* how_many_open){
//     int index, i;
//     long long min;
//     while (*how_many_open){
//         i = 0;
//         while (mas_file[i] == NULL){
//             i += 1;
//         }
//         min = num_mas[i];
//         index = i;
//         for (int j = i + 1; j < size; j++){
//             if (mas_file[j] != NULL && num_mas[j] < min){
//                 min = num_mas[j];
//                 index = j;
//             }
//         }
//         printf("%lld\n", num_mas[index]);
//         if (fscanf(mas_file[index], "%lld", &num_mas[index]) == -1){
//             fclose(mas_file[index]);
//             mas_file[index] = NULL;
//             *how_many_open -= 1;
//         }
//     }    
// }
void openn(FILE* mas_file[], int size, char** argv){
    for (int i = 0; i < size; i++){
        mas_file[i] = fopen(argv[i + 1], "r+");
    }
}
void read(FILE* mas_file[], int size, long long number_mas[], int state[]){
    for(int i = 0; i < size; i++){
        state[i] = fscanf(mas_file[i], "%lld", &number_mas[i]);
    }    
}

void print(FILE* mas_file[], int size, long long number_mas[], int state[]){
        int booll = 1;
    long long min;
    int index = 0;
    while(booll){
        min = LLONG_MAX;
        booll = 0;
        index = 0;
        for (int i = 0; i < size; i++){
            if(state[i] != EOF){
                booll = 1;
                if (number_mas[i] <= min){
                    min = number_mas[i];
                    index = i;
                }
            }
        }
        if (booll){
            printf("%lld\n", min);
            state[index] = fscanf(mas_file[index], "%lld", &number_mas[index]);
        }
    }
}
void closee(FILE* mas_file[], int size){
    for (int i = 0; i < size; i++){
        fclose(mas_file[i]);
    }
}

int main(int argc, char** argv){
    if (argc <= 1){
        return 0;
    }
    // int how_many_open = argc - 1;
    // long long num_mas[argc - 1];
    // FILE* mas_file[argc - 1];
    // input(num_mas, mas_file, argc - 1, argv, &how_many_open);
    //printf("%d", how_many_open);
    // print(num_mas, mas_file, argc - 1, argv, &how_many_open);
    FILE* mas_file[argc - 1];
    long long number_mas[argc - 1];
    int state[argc - 1];
    openn(mas_file, argc - 1, argv);
    read(mas_file, argc - 1, number_mas, state);
    print(mas_file, argc - 1, number_mas, state);
    closee(mas_file, argc - 1);
    return 0;
}