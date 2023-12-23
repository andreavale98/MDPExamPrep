#define _USE_MATH_DEFINES
#include <math.h>
#include<assert.h>
#include <vector>
#include<fstream>
#include<iostream>
#define pi M_PI

template <typename T>
struct mat {
	int rows_, cols_;
	std::vector<T> data_;

	mat(int rows = 0, int cols = 0) : rows_(rows), cols_(cols), data_(rows* cols) {}

	void resize(int rows, int cols) {
		rows_ = rows;
		cols_ = cols;
		data_.resize(rows * cols);
	}

	const T& at(int r, int c) const {
		assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		return data_[r * cols_ + c];
	}
	T& at(int r, int c) {
		return const_cast<T&>(static_cast<const mat*>(this)->at(r, c));
	}

	const T& operator()(int r, int c) const {
		assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		return data_[r * cols_ + c];
	}
	T& operator()(int r, int c) {
		assert(r >= 0 && r < rows_ && c >= 0 && c < cols_);
		return data_[r * cols_ + c];
	}

	int rows() const { return rows_; }
	int cols() const { return cols_; }
	int size() const { return rows_ * cols_; }

	size_t raw_size() const {
		return rows_ * cols_ * sizeof(T);
	}
	const char* raw_data() const {
		return reinterpret_cast<const char*>(&data_[0]);
	}
};

int main() {
	//prima n, poi k
	int n = 2048, k = 1024;
	mat<double> coses(n,k);
	for (size_t nn = 0; nn < n; nn++)
	{
		for (size_t kk = 0; kk < k; kk++) {
			coses(nn, kk) = cos(pi / n * (nn + 1 / 2 + n / 2) * (kk + 1 / 2));
		}
	}
	std::ifstream ifs("test.raw", std::ios::binary);
	short t;
	while (true)
	{
		ifs >> t;
		std::cout << t;
		if (!ifs) {
			break;
		}
	}

	return 0;
}