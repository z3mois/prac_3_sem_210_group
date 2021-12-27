#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
/*
В командной строке передаются два имени файлов.

Если оба имени указывают на один и тот же файл 
(помним, что один файл может иметь несколько имен), 
выдать диагностику и ничего делать, 
иначе первый файл — исходный, во второй записать результат обработки.
Обработка файла:
Файл исходный — не текстовый, бинарный. 
В файле содержатся 32-битные целые знаковые числа.
Переставить в исходном файле числа так, чтобы в начале файла
располагались отрицательные числа, а в конце — неотрицательные.
Порядок чисел среди чисел одного знака произвольный. 
Файл в память целиком не считывать, можно одновременно считывать не более 10 чисел файла. 
Временный файл создавать нельзя.
Использовать функции низкоуровневой работы с файлами.
В файл-результат записать текстовую версию обработанного файла
 — числа в текстовом виде через пробел.

*/
int int_in_curr_pos(int inp_file, int pos_in_file){
    lseek(inp_file, pos_in_file * 4, SEEK_SET);
    int number;
    read(inp_file, &number, 4);
    return number;
}

void print_number(int file, int number){
    long int degree_size = 1;
    int copy_number = number;
    while (copy_number){
        copy_number /= 10;
        degree_size *= 10;
    }
    //printf("%d\n", degree_size);
    char buf;
    //printf("%d %ld\n", number, degree_size);
    while (number){
        degree_size /= 10;
        buf = (int)number/degree_size + '0';//vosmojno perepolnenie poetomu long int i privedenie tipov
        number %= degree_size;
        write(file, &buf, 1);
    }
    buf = ' ';
    write(file, &buf, 1);
}
int main(int argc, char** argv){
    int inp_file = open(argv[1], O_RDWR, 0666);
    if (inp_file == -1){
        perror("can't open first file");
        return 0;
    }
    struct stat inp_stat;
    stat(argv[1], &inp_stat);
    int out_file = open(argv[2], O_RDWR|O_TRUNC|O_CREAT, 0666);
    struct stat out_stat;
    stat(argv[2], &out_stat);
    if (inp_stat.st_ino == out_stat.st_ino){
        perror("file1 equal file2");
        return 0;
    }
    int size = inp_stat.st_size;
    int number1;
    int number2;
    for (int i = 0; i < size/4 - 1; i++){
        for (int j = i; j < size/4; j++){
            //printf("%d %d\n", int_in_curr_pos(inp_file, j));
            if (int_in_curr_pos(inp_file, i) >= int_in_curr_pos(inp_file, j)){  
                lseek(inp_file, i * 4, SEEK_SET);
                read(inp_file, &number1, 4);
                lseek(inp_file, j * 4, SEEK_SET);
                read(inp_file, &number2, 4);
                lseek(inp_file, i * 4, SEEK_SET);
                write(inp_file, &number2, 4);
                lseek(inp_file, j * 4, SEEK_SET);
                write(inp_file, &number1, 4);
                //swap(a[i], a[j])
            }
        }
    }
    lseek(inp_file, 0, SEEK_SET);
    char sign;
    printf("output file:");
    while (read(inp_file, &number1, 4) == 4){
        printf("%d ", number1);
        if (number1 < 0){
            sign = '-';
            write(out_file, &sign, 1);
            print_number(out_file,-1 * number1);
        }
        else if (number1 == 0){
            sign = '0';
            write(out_file, &sign, 1);
            sign = ' ';
            write(out_file, &sign, 1);
        }
        else {
            print_number(out_file, number1);
        }
    }
    printf("\n");
    sign = '\n';
    write(out_file, &sign, 1);
    close(inp_file);
    close(out_file);
    return 0;
}
