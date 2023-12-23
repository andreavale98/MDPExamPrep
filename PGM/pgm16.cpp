#include "pgm16.h"
#include <fstream>
#include<iostream>



bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue) {
	std::ifstream is(filename, std::ios::binary);
	std::string magic;
	uint8_t byte1=0, byte2=0;
	char c;
	is >> magic;
	if (!is || magic != "P5") {
		std::cerr << "errore nell'aprire il file, o file del formato non corretto";
		return false;
	}
	is.get();
	c=is.peek();
	if (c == '#') {
		while (c!='\n')
		{
			c = is.get();
		}
	}
	is >> magic;
	int width=std::stoi(magic);
	is >> magic;
	int height = std::stoi(magic);
	img.resize(height, width);
	is >> magic;
	maxvalue = std::stoi(magic);
	c=is.get();
	if (maxvalue > 255) {
		for (int r = 0; r < img.rows(); r++)
		{
			for (int c = 0; c < img.cols(); c++) {
				is.read(reinterpret_cast<char*>(&byte1), 1);
				is.read(reinterpret_cast<char*>(&byte2), 1);
				img(r,c) = (byte1 << 8) + byte2;
			}
		}
		
	}
	else if(maxvalue>0){
		for (int r = 0; r < img.rows(); r++)
		{
			for (int c = 0; c < img.cols(); c++) {
				is.read(reinterpret_cast<char*>(&byte2), 1);
				img(r, c) = byte2;
			}
		}
	}
	else {
		return false;
	}
	return true;
}

/*
int main() {
	mat<uint16_t> img;
	uint16_t maxvalue;
	load("frog_bin.pgm",img,maxvalue);
	return 0;

}*/
