#include <iostream>
#include <fstream>
#include<cstdint>
#include <vector>

template <typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T))
{
	is.read((char*)&val, size);
	return is;
}

template <typename T>
std::ostream& raw_write(std::ostream& os, T& val, size_t size = sizeof(T))
{
	os.write((char*)&val, size);
	return os;
}

void read_literal(std::istream& is, uint8_t& tag, std::vector<uint8_t>& window, std::ostream& os, uint64_t &written_bytes) {
	uint8_t buffer;
	uint32_t length = (tag>>2);
	if (length==60)
	{
		raw_read(is, length, 1);

	}
	else if (length == 61) {
		raw_read(is, length, 2);
	}
	else if (length == 62) {
		raw_read(is, length, 3);
	}
	else if(length==63) {
		raw_read(is, length, 4);
	}


	if (!is) return;
	length++; //ho letto l-1, voglio l+1


	for (size_t i = 0; i < length; i++)
			{
				raw_read(is, buffer);
				if (!is)
				{
					break;
				}
				raw_write(os, buffer);
				written_bytes++;
				window.push_back(buffer);
			
			}

	return;
}

void read_copy_1_byte_offset(std::istream& is, uint8_t& tag, std::vector<uint8_t>& window, std::ostream& os, uint64_t& written_bytes) {
	uint8_t mask_length = 28; //0b00011100
	uint8_t mask_offset = 0b11100000;
	uint8_t buffer;
	uint16_t length;
	uint32_t offset;


	length = (tag & mask_length)>>2;
	length += 4;
	raw_read(is, buffer);
	offset = (mask_offset & tag)>>5;
	offset = (offset << 8) + buffer;
	uint64_t start = window.size() - offset;

	//error check

	if (offset>window.size()|| length<4|| length>11 || offset<0 || offset>2047)
	{
		std::cerr << "errore";
		exit(1);
		return;
	}
	for (size_t i = start; i < start+length; i++)
	{
		window.push_back(window[i]);
		raw_write(os,window[i]);
		written_bytes++;
	}

}
void read_copy_2_byte_offset(std::istream& is, uint8_t& tag, std::vector<uint8_t>& window, std::ostream& os, uint64_t& written_bytes) {

	uint8_t mask_length = 0b11111100;
	uint16_t length;
	uint16_t offset;


	length = (tag & mask_length)>>2;
	length += 1;
	raw_read(is, offset);
	uint64_t start = window.size() - offset;

	//error check

	if (offset > window.size() || length < 1 || length>64 || offset < 0 || offset>65535)
	{

		exit(1);
		return;
	}

	for (size_t i = start; i < start + length; i++)
	{

		window.push_back(window[i]);
		raw_write(os, window[i]);
		written_bytes++;
	}


}
void read_copy_4_byte_offset(std::istream& is, uint8_t& tag, std::vector<uint8_t>& window, std::ostream& os,uint64_t& written_bytes) {
	uint8_t mask_length = 0b11111100;
	uint16_t length;
	uint32_t offset;


	length = (tag & mask_length)>>2;
	length += 1;
	raw_read(is, offset);
	uint64_t start = window.size() - offset;

	//error check

	if (offset > window.size() || length < 1 || length>64)
	{
		std::cerr << "errore, copy 4 byte offset";
		exit(1);
	}

	for (size_t i = start; i < start + length; i++)
	{
		window.push_back(window[i]);
		raw_write(os, window[i]);
		written_bytes++;
	}


}



void snappy_decomp(std::ifstream& is, std::ofstream& os) {
	uint64_t og_stream_size=0;
	uint64_t written_bytes=0;
	uint8_t byte_buffer;
	uint8_t byte_letti = 0;
	uint64_t big_buffer;
	bool read_another_byte = true;
	std::vector<uint8_t> window;

	//read stream size
	while (1)
	{
		raw_read(is, byte_buffer);
		if (byte_buffer<128) //se il primo bit non è 1, allora è l'ultimo
		{
			read_another_byte=false;
			big_buffer = byte_buffer;
		}
		else
		{
			big_buffer = byte_buffer - 128; //rimuovo l'1 come primo bit così rimangolo solo i bit interessanti
		}
		

		og_stream_size += big_buffer<<(7*byte_letti); //primo byte letto sarà il meno significativo
		byte_letti++;

		if (!read_another_byte)
		{
			break;
		}
		
	}

	uint8_t tag;
	uint16_t tag_type = 0;

	while (written_bytes<og_stream_size)
	{
		raw_read(is, tag);
		if (!is)
		{
			break;
		}
		tag_type = tag & 3;
		switch (tag_type)
		{
		break; case 0: read_literal(is,tag,window,os,written_bytes);
		break; case 1: read_copy_1_byte_offset(is, tag, window, os,written_bytes);
		break; case 2: read_copy_2_byte_offset(is, tag, window, os,written_bytes);
		break; case 3: read_copy_4_byte_offset(is, tag, window, os,written_bytes);
		}
		if (!is)
		{
			break;
		}
	}

	return;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "usage:     snappy_decomp <input file> <output file>";
	}
	std::ifstream is(argv[1],std::ios::binary);
	std::ofstream os(argv[2], std::ios::binary);
	is.peek();
	if (!is || !os) {
		std::cerr << "input file does not exist!";
		exit(1);
	}
	snappy_decomp(is, os);

}