#include<stdio.h>
#include<stdlib.h>

/*
Написать функцию, параметры 2-мерный массив из char, размерности n и m, число char x.
«Закрасить» символом ‘#’ те строки и столбцы массива, на пересечении которых стоит число, равное данному x. Можно считать, что в исходном массиве нет ‘#’.
Сделать 2 варианта функции:
а) Создать новый измененный массив. Вернуть его адрес.
б) Менять сам массив. Дополнительное условие - можно выделить дополнительно только 1 строку или 1 столбец.
Нужна также функция для вывода на печать 2-мерного массива.
В main() задать массив, вывести исходный массив на экран. Продемонстрировать работу функций.
*/
void Inputmatrix(int n, int m, char** matrix){

    char temp;
    scanf("%c", &temp);
    for (int i = 0; i  < n; i++){
        for (int j = 0; j < m; j++){
            scanf("%c", &matrix[i][j]);
            //printf("%c", matrix[i][j]);
        }
        scanf("%c", &temp);
    }
    printf("\n");
};

void outputmatrix(int n, int m, char** matrix){
    for (int i = 0; i  < n; i++){
        for (int j = 0; j < m; j++){
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
};


char** create(int n, int m, char** matrix, char x){
    char** newmatrix = (char**)malloc(n * sizeof(char*));
    for(int i = 0; i < n; i++){
        newmatrix[i] = (char*)malloc(m * sizeof(char));
    }
    for (int i = 0; i  < n; i++){
        for (int j = 0; j < m; j++){
            newmatrix[i][j] = matrix[i][j];
        }
    }
    for (int i = 0; i  < n; i++){
        for (int j = 0; j < m; j++){
                if (newmatrix[i][j] == x)  {
                    for(int f = i - 1;  f >=0; f--){
                        if (newmatrix[f][j] != x)
                            newmatrix[f][j] = '#';
                    }
                    for(int f = i + 1;  f < n; f++){
                        if (newmatrix[f][j] != x)
                            newmatrix[f][j] = '#';
                    }
                    for(int f = j - 1;  f >=0; f--){
                        if (newmatrix[i][f] != x)
                            newmatrix[i][f] = '#';
                    }
                    for(int f = j + 1;  f < m; f++){
                        if (newmatrix[i][f] != x)
                            newmatrix[i][f] = '#';
                    }
                }

        }
    }
    for (int i = 0; i  < n; i++){
        for (int j = 0; j < m; j++){
            if (newmatrix[i][j] == x)
                newmatrix[i][j] = '#';
        }
    }
    return newmatrix;
}
void create1(int n, int m, char** matrix, char x){
    for (int i = 0; i  < n; i++){
        for (int j = 0; j < m; j++){
                if (matrix[i][j] == x)  {
                    for(int f = i - 1;  f >=0; f--){
                        if (matrix[f][j] != x)
                            matrix[f][j] = '#';
                    }
                    for(int f = i + 1;  f < n; f++){
                        if (matrix[f][j] != x)
                            matrix[f][j] = '#';
                    }
                    for(int f = j - 1;  f >=0; f--){
                        if (matrix[i][f] != x)
                            matrix[i][f] = '#';
                    }
                    for(int f = j + 1;  f < m; f++){
                        if (matrix[i][f] != x)
                            matrix[i][f] = '#';
                    }
                }

        }
    }
    for (int i = 0; i  < n; i++){
        for (int j = 0; j < m; j++){
            if (matrix[i][j] == x)
                matrix[i][j] = '#';
        }
    }
}
int main(){
    int n, m;
    printf("enter rows and column ");
    scanf("%d%d", &n, &m);
    printf("enter a string of characters without spaces, at the end a line break");
    char** matrix = (char**)malloc(n * sizeof(char*));
    for(int i = 0; i < n; i++){
        matrix[i] = (char*)malloc(m * sizeof(char));
    }
    printf("enter matrix\n");

    Inputmatrix(n, m, matrix);
    outputmatrix(n, m, matrix);
    printf("enter x \n");
    char x;
    scanf("%c", &x);
    char** newm = create(n, m, matrix, x);
    printf("new matrix\n");
    outputmatrix(n, m, newm);
    create1(n, m, matrix, x);
    printf("old matrix\n");
    outputmatrix(n, m, matrix);
    for (int i = 0; i < n; ++i){
        free(newm[i]);
    }
    free(newm);
    for (int i = 0; i < n; ++i){
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}
