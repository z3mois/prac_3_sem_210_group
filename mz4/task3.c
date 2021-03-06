#include<stdio.h>
#include<math.h>
// На стандартном потоке ввода подаются числа a, b и n. 
// Числа a и b представимы типом int.
// Число n представимо типом int и больше 0.
// Число n задает ширину ячейки таблицы в символах.
// Числа a и b — это границы диапазона целых чисел [a;b). 
// Гарантируется, что a < b.
// На стандартный поток вывода напечатайте таблицу умножения для этих 
// чисел как показано в примере. Ячейки таблицы, расположенные на одной 
// строке, разделяются строго одним пробелом. 
// В конце каждой строки выводится символ \n. 
// Пробелы в конце строки должны отсутствовать. 
// Гарантируется, что ширина ячейки таблицы достаточна для
//  вывода произведения соответствующих строк и столбца.
// Вывод вашей программы будет сравниваться с правильным ответом побайтово.
//  Все пробельные символы — значимы.
// Указание: для вывода числа с требуемым количеством пробелов используйте 
// спецификацию * ширины поля вывода printf.
int main() {
    int a, b;
    unsigned n;
    scanf("%d%d%u", &a, &b, &n);
    printf("%*c", n, ' ');
    for (int i = a; i < b; ++i){
        printf("%*lld", n + 1, (long long)i);
    }
    printf("\n");
    for (int i = a; i < b; i++){
        printf("%*lld", n, (long long)i);
        for (int j = a; j < b; j++){
            printf("%*lld", n + 1, (long long)i * j);
        }
        printf("\n");
    }
    //     int i, j;
    // printf("%*c", n+1, ' ');
    // for(i = a; i < b; i++)
    //     if(i == (b - 1))
    //         printf("%*d", n, i);
    //     else
    //         printf("%*d ", n, i);
    // printf("\n");
    // for(i = a; i < b; i++){
    //     printf("%*d ", n, i);
    //     for(j = a; j < b; j++)
    //         if(j == (b - 1))
    //             printf("%*lld", n, (long long)(i*j));
    //         else
    //             printf("%*lld ", n, (long long)(i*j));
    //     printf("\n");
    // }
    return 0;
}