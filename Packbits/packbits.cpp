#include <fstream>
#include <string.h>
#include <cstdlib>
#include <vector>

#include<algorithm>
template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}

template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}

void compress_copy(std::istream& is, std::ostream& os,uint8_t& value1,uint8_t& value2) {
	uint8_t copy_length = 2;
	std::vector<uint8_t> buffer = {value1,value2};
	raw_read(is, value1);

	// casistica in cui gli ultimi due caratteri del file sono diversi
	if (!is) {
		copy_length = buffer.size();
		uint8_t header = copy_length - 1;
		raw_write(os, header);
		raw_write(os, buffer[0], buffer.size());
		return;
	}
	int peek;
	while (value2 != value1)
	{
		// casistica copy into run
		peek = is.peek();
		if (peek == value1)
			break;
		buffer.push_back(value1);
		copy_length++;
		if (copy_length == 128) {
			uint8_t header = copy_length-1;
			raw_write(os, header);
			raw_write(os, buffer[0], buffer.size());
			buffer.clear();
		}

		value2 = value1;
		raw_read(is, value1);
		if (!is) {
			break;
		}

	}
	copy_length = buffer.size();
	if (copy_length == 0) {
		return;
	}
	uint8_t header = copy_length - 1;
	raw_write(os, header);
	raw_write(os, buffer[0],buffer.size());
	buffer.clear();
	return;

}

void compress_run(uint8_t& value1, uint8_t& value2, uint8_t& repetition, uint8_t& header, std::ofstream& os, std::ifstream& is)
{
	while (value1 == value2)
	{
		
		raw_read(is, value2);
		if (!is) {
			repetition++;
			break;
		}
		repetition++;
		if (repetition == 128) {
			header = 257 - repetition;
			raw_write(os, header);
			raw_write(os, value1);
			value1 = value2;
			repetition = 1;
			return;
		}

	}

	header = 257 - repetition;
	raw_write(os, header);
	raw_write(os, value1);
	value1 = value2;
	repetition = 1;
}

void encode_packbits(const char* infile, char* outfile) {
	uint8_t value1,value2,repetition=1;
	uint8_t header;

	std::ifstream is = std::ifstream(infile, std::ios::binary);
	std::ofstream os = std::ofstream(outfile, std::ios::binary);
	raw_read(is, value1);
	if (!is) {
		printf("problemi nella lettura");
		is.close();
		exit(1);
	}


	while (1)
	{
		// controllo sulla rottura dello stream dato che ad ogni funzione c'è una read
		raw_read(is, value2);
		if (!is) {
			break;
		}
		if (value1!=value2)
		{
			compress_copy(is, os,value1,value2);
			
		}
		// controllo sulla rottura dello stream dato che ad ogni funzione c'è una read
		if (!is) {
			break;
		}
		if (value1==value2)
		{
			compress_run(value1, value2, repetition, header, os, is);
		}
		// controllo sulla rottura dello stream dato che ad ogni funzione c'è una read
		if (!is) {
			break;
		}
		
		

	}
	uint8_t eof = 128;
	raw_write(os, eof);
	
}

void decode_packbits(const char* infile, char* outfile){
	uint8_t header, value;
	std::ifstream is = std::ifstream(infile, std::ios::binary);
	std::ofstream os = std::ofstream(outfile, std::ios::binary);
	raw_read(is, header);
	if (!is) {
		printf("problemi nella lettura");
		is.close();
		exit(1);
	}
	while (1)
	{
		if (header < 128 && header>0) {
			std::vector<uint8_t>buffer(header+1);
			raw_read(is, buffer[0], buffer.size());
			if (!is) {
				break;
			}
			raw_write(os, buffer[0], buffer.size());

		}
		else if (header > 128 && header <= 255) {
			std::vector<uint8_t>buffer(257-header);
			raw_read(is, buffer[0]);
			
			if (!is) {
				break;
			}
			std::fill(buffer.begin(),buffer.end(),buffer[0]);
			raw_write(os, buffer[0],buffer.size());
		}
		else
		{
			break;
		}
		raw_read(is, header);
		if (!is)
		{
			break;
		}
	}

}

int main(int argc, char** args) {
	if (argc != 4) {
		printf("usage packbits [c|d] <input file> <output file>");
		exit(1);
	}
	if (strcmp(args[1],"c")==0)
	{
		encode_packbits("compressme3.txt", args[3]);
	}
	if (strcmp(args[1], "d") == 0)
	{
		decode_packbits(args[2], args[3]);
	}
}