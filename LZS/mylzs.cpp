#include "lzs.h"
#include<cstdint>
#include <vector>
#include<iostream>

template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}

class bitreader {
	std::istream& is_;
	uint8_t buf_;
	uint8_t len_;
public:
	bitreader(std::istream& is) : is_(is), len_(0) {

	}

	uint8_t read_bit() {
		if (len_ > 0) {
			uint8_t ret = (buf_ >> (len_ - 1)) & 1;
			len_--;
			return ret;
		}
		else {
			len_ = 8;
			is_.read((char*)&buf_, 1);
			return read_bit();
		}
	}

	uint64_t read_bits(uint8_t size) {
		uint64_t ret = 0;
		do {
			ret += read_bit() << size - 1;
		} while (--size);

		return ret;
	}
};

void lzs_decompress(std::istream& is, std::ostream& os) {
	std::vector<uint8_t> window;
	bitreader br(is);
	bool head;
	uint8_t byte;
	uint32_t offset;
	uint32_t length;
	uint32_t Fcount;
	uint32_t bytecounter;
	std::vector<uint8_t> buffer;
	bool debug = true;
	bool offset_bit;
	int iter=0;

	while (1)
	{
		head = br.read_bit();
		if (!is) {
			break;
		}
		if (head == 0)
		{
			byte = br.read_bits(8);
			window.push_back(byte);
			os.write(reinterpret_cast<char*>(&byte), 1);

		}
		else
		{
			offset_bit = br.read_bit();
			if (offset_bit==1)
			{
				offset = br.read_bits(7);
				if (offset == 0) {
					break;
				}
			}
			else
			{
				offset = br.read_bits(11);
			}
			//calcolo length
			length = br.read_bits(2);
			if (length >= 0 && length <= 2)
			{
				length += 2;
			}
			else {
				length = ((length << 2) + br.read_bits(2))-7;
				if (length == 8) {
					Fcount = 0;
					length + 7;
					while (length == 15)
					{
						Fcount++;
						length = br.read_bits(4);

					}
					length = length + (Fcount * 15 - 7);
				}
			}

			
			bytecounter = 0;
			if (offset>window.size())
			{
				offset = window.size();
			}
			while (length > 0)
			{
				buffer.push_back(window.at(window.size() - (offset - bytecounter % offset)));
				length--;
				bytecounter++;
			}
			window.insert(std::end(window), std::begin(buffer), std::end(buffer));
			os.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
			buffer.clear();




		}
		if (debug)
		{
			std::cout << "iterazione numero: " << iter;
			iter++;
		}
	}

	return;
}