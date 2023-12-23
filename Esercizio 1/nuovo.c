/*

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

int cmpfunc(const void* a, const void* b)
{
    int32_t x = *(int32_t*)a;
    int32_t y = *(int32_t*)b;
    if (x < y) {
        return -1;
    }
    else if (x > y) {
        return 1;
    }
    else {
        return 0;
    }
}

typedef struct vector {
    int32_t* nums;
    int n;
    int capacity; // capacity of nums array
} vector;

void vector_constructor(vector* this) {
    this->nums = NULL;
    this->n = 0;
    this->capacity = 0;
}
void vector_destructor(vector* this) {
    free(this->nums);
}
void vector_push_back(vector* this, int32_t num) {
    if (this->n == this->capacity) {
        this->capacity = (this->capacity == 0 ? 1 : this->capacity * 2);
        this->nums = realloc(this->nums, this->capacity * sizeof(int32_t));
        if (this->nums == NULL) {
            printf("Error: failed to allocate memory.\n");
            exit(EXIT_FAILURE);
        }
    }
    this->nums[this->n] = num;
    this->n++;
}
void vector_sort(vector* this) {
    qsort(this->nums, this->n, sizeof(int32_t), cmpfunc);
}
int vector_size(const vector* this) {
    return this->n;
}
int32_t vector_at(const vector* this, int i) {
    assert(i >= 0 && i < this->n);
    return this->nums[i];
}


int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: sort_int <filein.txt> <fileout.txt>\n");
        return 1;
    }

    FILE* fin = fopen("file02.txt", "r");
    if (fin == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    FILE* fout = fopen(argv[2], "w");
    if (fout == NULL) {
        printf("Error opening output file.\n");
        fclose(fin);
        return 1;
    }

    vector v;
    vector_constructor(&v);

    while (1) {
        int32_t num;
        if (fscanf(fin, "%" SCNd32, &num) == 1) {
            vector_push_back(&v, num);
        }
        else if (feof(fin)) {
            break;
        }
        else {
            printf("Warning: incorrect data in input file.\n");
            break;
        }
    }

    vector_sort(&v);

    for (int i = 0; i < vector_size(&v); i++) {
        fprintf(fout, "%" PRId32 "\n", vector_at(&v, i));
    }

    vector_destructor(&v);
    fclose(fin);
    fclose(fout);

    return 0;
}
*/