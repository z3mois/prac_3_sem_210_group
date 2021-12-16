#include<stdio.h>

/*
Написать функцию, параметры — массив целых и размерность массива.
Функция должна отсортировать элементы данного массива по возрастанию количества единичных бит в числе.
В случае равенства количества единичных бит сортировать по убыванию значения числа.
Пример. {1, 2, 3, 4, 5} => {4, 2, 1, 5, 3}
В main() задать массив, вывести массив на экран как последовательность чисел в десятичном и в двоичном виде (использовать Вашу функцию печати).
Вызвать функцию сортировки, затем вывести отсортированный массив.
*/
void second(unsigned int x){
	unsigned int one = 1;
	one <<= 31;
	for(int i = 0; i < 32; ++i) {
		printf("%d", (x & one) ? 1 : 0);
		one >>= 1;
	}
}
void outarray10(int n, int mas[]){
    for (int i = 0; i < n; i++){
       printf("%d ", mas[i]);
    }
    printf("\n");
}
void outarray2(int n, int mas[]){
    for (int i = 0; i < n; i++){
       second(mas[i]);
       printf("\n");
    }
    printf("\n");
}

int first(int x){
    int one = 1;
    int count = 0;
	for(int i = 0; i < 32; ++i) {
		if (x & one)
            count++;
		x >>= 1;
	}
	return count;
}

void sort(int n, int mas[]){
    int temp;
    for (int i = 0; i < n - 1; i++){
        for (int j = i + 1; j < n; j++){
            if (first(mas[i]) > first(mas[j])) {
                temp = mas[i];
                mas[i] = mas[j];
                mas[j] = temp;
            }
            else if (first(mas[i]) == first(mas[j])) {
                if (mas[i] < mas[j]) {
                    temp = mas[i];
                    mas[i] = mas[j];
                    mas[j] = temp;
                }
            }
        }
    }
}
int main() {
    int n;
    printf("enter n: ");
    scanf("%d", &n);
    int mas[n];
    for (int i = 0; i < n; i++){
        scanf("%d", &mas[i]);
    }
    outarray10(n, mas);
    outarray2(n, mas);
    sort(n, mas);
    outarray10(n, mas);
    outarray2(n, mas);
    return 0;
}
