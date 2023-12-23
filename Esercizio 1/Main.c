#include <stdio.h>
#include<stdlib.h>
#define MAXNUM 1000

int cmpfunct(const void * a, const void * b) {
	if (*(int*)a > *(int*)b)
		return 1;
	else if (*(int*)a < *(int*)b)
		return -1;
	return 0;
}
int main(int argc, char** args) {
	if (argc != 3) {
		printf("inserisci 2 parametri");
		return 1;
	}
	FILE* fp = fopen("file02.txt", "r");
	FILE* fout = fopen(args[2], "w");
	if (fp == NULL || fout==NULL) {
		printf("errore");
	}
	int buffer;
	int *array = malloc(sizeof(int));
	size_t capacity = 1;
	size_t n=0;
	while (1)
	{
		if (fscanf(fp, "%d", &buffer) != 1) {
			if (feof(fp))
			{
				break;
			}
			else
			{
				break;
			}

		}
		else {

			if (n == capacity)
			{
				capacity *= 2;
				array = realloc(array, 2 * capacity * sizeof(int));
				if (array == NULL) {
					printf("errore");
					exit(EXIT_FAILURE);
				}
			}
			array[n] = buffer;
			n++;
		}
		

	}

	qsort(array, n, sizeof(int), cmpfunct);
	for (size_t i = 0; i < n; i++)
	{
		fprintf(fout, "%d\n", array[i]);
	}
	return 0;

}