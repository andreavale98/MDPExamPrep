#include"mat.h"
#include"pgm.h"
#include "ppm.h"
#include"types.h"
#include"utils.h"
#include<iostream>
#include <sstream>
#include<iomanip>
#include<fstream>

void tokenize(std::string const& str, const char delim,
	std::vector<std::string>& out)
{
	// construct a stream from the string 
	std::stringstream ss(str);

	std::string s;
	while (std::getline(ss, s, delim)) {
		out.push_back(s);
	}
}

bool y4m_extract_gray(const std::string& filename,std::vector<mat<uint8_t>>& frames) 
{
	int height, width;
	std::ifstream is(filename, std::ios::binary);
	std::string magic,chroma;
	uint8_t c;
	is >> magic;
	if (magic!="YUV4MPEG2" || !is)
	{
		std::cerr << "errore nel leggere il file o file del formato non corretto";
		exit(1);
	}
	while (true)
	{
		c = is.get();
		if (c == '\n') {
			std::cout << "parsato lo stream header";
			break;
		}
		if (c=='H')
		{
			is >> height;
		}
		if (c == 'W')
		{
			is >> width;
		}
		if (c=='C')
		{
			is >> chroma;
			if (chroma!="420jpeg")
			{
				printf("chroma non supportato");
				exit(1);
			}
		}
	}
	return false;
}
int main() {

	std::vector<mat<uint8_t>> frames;
	if (y4m_extract_gray("test1.y4m", frames)) {
		for (uint8_t i = 0; i < frames.size(); i++)
		{
			std::stringstream ss;
			ss << std::setw(3) << std::setfill('0');
			ss << "frame" << i << ".pgm";

		}
	}
	return 0;
}