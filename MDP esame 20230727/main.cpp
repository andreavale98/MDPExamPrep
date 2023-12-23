#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <bitset>
#include <vector>

template <typename T>
std::istream& raw_read(std::istream& is, T& val, size_t s = sizeof(T)) {
	return is.read((char*) & val, s);
}

template <typename T>
std::ostream& raw_write(std::ostream& os, T& val, size_t s = sizeof(T)) {
	return os.write((char*)&val, s);
}

/*
*In general, every opcode instructs the decoder to firstly add some bytes (literals) to the output stream, 
and then to go back in the already decoded stream of a certain match distance and append match length bytes to the decoded stream. 

*/

void write_cose(std::istream&is, std::ostream&os, std::vector<uint8_t>& stream_decompresso, uint32_t match_distance, uint32_t literal_length, uint32_t match_length)
{
	uint8_t buffer;
	size_t start = stream_decompresso.size();
	if (literal_length>0)
	{
		for (size_t i = 0; i < literal_length; i++) {
			raw_read(is, buffer);
			stream_decompresso.push_back(buffer);
		}
		raw_write(os, stream_decompresso[start], literal_length);
	}
	if (match_length==0 || match_distance==0)
	{
		return;
	}
	start = stream_decompresso.size() - match_distance;
	if (start>=stream_decompresso.size())
	{
		std::cout<<"errore";
		exit(1);
	}
	for (size_t i = 0; i < match_length; i++) {
			buffer=stream_decompresso[start+i];
			stream_decompresso.push_back(buffer);
			raw_write(os, buffer);
	}
}

void lzvn_decode(std::istream& is, std::ostream& os) {
	uint8_t id[4];
	for (size_t i = 0; i < 4; i++)
	{
		raw_read(is, id[i]);
	}
	
	uint32_t output_size,block_size;
	raw_read(is, output_size);
	raw_read(is, block_size);
	uint8_t opcode;
	std::string operation;
	
	uint8_t mask_4_bits = 0xF;
	uint8_t partial_match;
	uint8_t distance_match;
	uint32_t match_distance;
	uint32_t literal_length;
	uint32_t match_length;
	std::vector<uint8_t> stream_decompresso;
	uint8_t buffer;


	while (1)
	{
		operation = "don't know yet!";
		raw_read(is, opcode);
		if (!is) {
			break;
		}
		switch (opcode)
		{
		break; case 0b11110000: operation = "large match"; 
		break; case 0b11100000:operation = "large literal";
		break; case 0b00001110: operation = "nop"; continue;
		break; case 0b00010110: operation = "nop"; continue;
		break; case 0b00000110: operation = "eos";
		break; case 0b00011110: operation = "udef";
		break; case 0b00100110: operation = "udef";
		break; case 0b00101110: operation = "udef";
		break; case 0b00110110: operation = "udef";
		break; case 0b00111110: operation = "udef";
		default:
			break;
		}
		if (operation=="large match")
		{
			raw_read(is, buffer);
			match_length = buffer+16;
			write_cose(is, os, stream_decompresso, match_distance, 0, match_length);
			continue;
		}
		else if (operation == "large literal") {
			uint8_t buffer;
			raw_read(is, buffer,1);
			literal_length = buffer+16;
			write_cose(is, os, stream_decompresso, 0, literal_length, 0);
			continue;
		}
		else if (operation == "eos") {
			break;
		}
		else if (operation=="udef")
		{
			std::cerr << "undefined opcode encoutered";
			exit(1);
		}

		partial_match = (opcode >> 4) & mask_4_bits;
		switch (partial_match)
		{
		break; case 0b1111: operation = "small match";
		break; case 0b1110: operation = "small literal";
		break; case 0b0111: operation = "udef";
		break; case 0b1101: operation = "udef";
		default:
			break;
		}

		if (operation=="small match")
		{
			match_length = opcode & 0xF;
			write_cose(is, os, stream_decompresso, match_distance, 0, match_length);
			continue;
		}else if(operation=="small literal"){
			literal_length = opcode&0xF;
			write_cose(is, os, stream_decompresso, 0, literal_length, 0);
			continue;
		}
		else if (operation=="udef")
		{
			std::cerr << "undefined opcode encoutered";
			exit(1);
		}

		// medium distance?
		distance_match = (opcode >> 5) & 0b111;
		if (distance_match==0b101)
		{
			literal_length = (opcode >> 3) & 0b11;
			match_length = opcode & 0b111;
			match_length=match_length << 2;
			raw_read(is, opcode);
			buffer = opcode & 0b11;
			match_length += buffer+3;
			
			buffer = (opcode >> 2) & 0b111111;
			raw_read(is, opcode);
			match_distance = opcode << 6;
			match_distance += buffer;

			operation = "medium distance";

			if (match_distance>16383)
			{
				std::cout << "errore medium distance, match distance troppo lunga";
				exit(1);
			}
			if (match_length < 3 || match_length>34) {
				std::cout << "errore medium distance, match lenght troppo lunga";
				exit(1);
			}

			write_cose(is, os, stream_decompresso, match_distance, literal_length, match_length);
			continue;
		}
		distance_match = opcode & 0b111;
		if (distance_match==0b111)
		{

			literal_length = (opcode >> 6) & 0b11;

			match_length = (opcode >> 3) & 0b111;
			match_length += 3;

			raw_read(is, match_distance, 2);
			operation = "large distance";

			write_cose(is, os, stream_decompresso, match_distance, literal_length, match_length);


			continue;

		}
		else if (distance_match == 0b110) {

			literal_length = (opcode >> 6) & 0b11;
			match_length = (opcode >> 3) & 0b111;
			match_length += 3;
			operation = "previous distance";

			write_cose(is, os, stream_decompresso, match_distance, literal_length, match_length);

			
			continue;
		}
		operation = "small distance";
		literal_length = (opcode >> 6) & 0b11;
		match_length = ((opcode >> 3) & 0b111);
		match_length += 3;
		match_distance = (opcode & 0b111)<<8;
		raw_read(is, opcode);
		match_distance += opcode;
		write_cose(is,os, stream_decompresso, match_distance, literal_length, match_length);

	}

}

int main(int argc, char** argv) {

	std::ifstream is(argv[1], std::ios::binary);
	std::ofstream os(argv[2], std::ios::binary);
	is.peek();

	if (argc!=3)
	{
		std::cerr << "usage: lzvn_decode <input file> <output file>";
		return 1;
	}

	if (!is) {
		std::cerr << "input file does not exist!";
		return 1;
	}

	lzvn_decode(is, os);



	return 0;
}