#include "lzs.h"
int main() {
	std::ifstream is("bibbia.txt.lzs", std::ios::binary);
	std::ofstream os("mybibbia.txt", std::ios::binary);
	lzs_decompress(is, os);
}