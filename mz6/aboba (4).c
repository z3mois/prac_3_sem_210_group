#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define ushort unsigned short
// В аргументе командной строки задается имя файла. 
// Файл содержит 16-битные беззнаковые целые числа в представлении Big Endian в бинарном виде.
// На стандартный поток вывода выведите минимальное число среди четных чисел в этом файле. 
//Вывод завершайте символом перехода на новую строку. 
//Если в файле отсутствуют четные числа, программа не должна выводить ничего.
// Размер файла всегда кратен 2.
int fsize(const char *file_name) {
	FILE* fptr = fopen(file_name, "rb");
    	fseek(fptr, 0L, SEEK_END);
	int sz = ftell(fptr);
	fclose(fptr);
	return sz;
}

void init_file(const char *file_name, int num_count) {
	FILE* fptr = fopen(file_name, "wb");
	ushort* arr = (ushort*)malloc(sizeof(ushort) * num_count);
	for (int i = 0; i < num_count; ++i) {
		arr[i] = rand() % 32;
		
	}
	fwrite(arr, sizeof(ushort), num_count, fptr);
	free(arr);
}

int main(int argc, char** argv) {
	char* file_name = argv[1];

	FILE* fptr = fopen(file_name, "rb");
	int file_size = fsize(file_name);
	int num_count = file_size / 2;
	ushort* arr = (ushort*)malloc(sizeof(ushort) * num_count);
	fread(arr, sizeof(ushort), num_count, fptr);
	char* arr2 = (char*)arr;
	for (int i = 0; i < num_count; ++i) {
		char first = arr[i] % 256;
		char second = arr[i] / 256;
		arr2[i*2] = second;
		arr2[i*2 + 1] = first;
	}

	int found = FALSE;
	ushort answer = -1; //max
	for (int i = 0; i < num_count; ++i) {
		if (arr[i] % 2 == 0 && answer > arr[i]) {
			answer = arr[i];
			found = TRUE;
		}
	}
	
	if (found)
		printf("%d\n", answer);

	fclose(fptr);
	free(arr);
}
