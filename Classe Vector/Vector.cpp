#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

#include <utility>

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

struct vector {
    int32_t* nums_;
    int n_;
    int capacity_; // capacity of nums array

    vector() {
        nums_ = NULL;
        n_ = 0;
        capacity_ = 0;
    }
    vector(int initial_size) {
        nums_ = (int32_t*)calloc(initial_size, sizeof(int32_t));
        n_ = initial_size;
        capacity_ = initial_size;
    }
    vector(const vector& other) {
        n_ = other.n_;
        capacity_ = other.capacity_;
        nums_ = (int32_t*)malloc(capacity_ * sizeof(int32_t));
        for (size_t i = 0; i < n_; ++i) {
            nums_[i] = other.nums_[i];
        }
    }
    vector& operator=(vector other) { // copy-and-swap idiom
        swap(*this, other);
        return *this;
    }
    friend void swap(vector& left, vector& right) {
        using std::swap;
        swap(left.n_, right.n_);
        swap(left.capacity_, right.capacity_);
        swap(left.nums_, right.nums_);
    }
    ~vector() {
        free(nums_);
    }
    void push_back(int32_t num) {
        if (n_ == capacity_) {
            capacity_ = (capacity_ == 0 ? 1 : capacity_ * 2);
            nums_ = (int32_t*)realloc(nums_, capacity_ * sizeof(int32_t));
            if (nums_ == NULL) {
                printf("Error: failed to allocate memory.\n");
                exit(EXIT_FAILURE);
            }
        }
        nums_[n_] = num;
        n_++;
    }
    void sort() {
        qsort(nums_, n_, sizeof(int32_t), cmpfunc);
    }
    int size() const {
        return n_;
    }
    int32_t at(int i) const {
        assert(i >= 0 && i < n_);
        return nums_[i];
    }

    int32_t& operator[](int i) {
        return nums_[i];
    }
    const int32_t& operator[](int i) const {
        return nums_[i];
    }

};

void raddoppia(int& val) {
    val *= 2;
}

void scrivi_vettore(FILE* f, const vector& v) {
    for (int i = 0; i < v.size(); i++) {
        fprintf(f, "%" PRId32 "\n", v[i]);
    }
}


int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: sort_int <filein.txt> <fileout.txt>\n");
        return 1;
    }

    FILE* fin = fopen(argv[1], "r");
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

    while (1) {
        int32_t num;
        if (fscanf(fin, "%" SCNd32, &num) == 1) {
            v.push_back(num);
        }
        else if (feof(fin)) {
            break;
        }
        else {
            printf("Warning: incorrect data in input file.\n");
            break;
        }
    }

    vector x(10);

    x = v;

    v[0] = 5;

    v.sort();

    scrivi_vettore(fout, v);

    fclose(fin);
    fclose(fout);

    return 0;
}
