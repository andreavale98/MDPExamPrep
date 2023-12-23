#include"pcx.h"
#include<iostream>

bool parse_header(std::ifstream &is, mat<uint8_t>& img, pcx& of) {
	uint8_t byte_letti=6;
	raw_read(is, of.manifacturer_);
	raw_read(is, of.version_);
	raw_read(is, of.encoding_);
	raw_read(is, of.bitsperplane_);
	raw_read(is, of.WindowXmin_);
	raw_read(is, of.WindowYmin_);
	raw_read(is, of.WindowXmax_);
	raw_read(is, of.WindowYmax_);
	is.ignore(6);
	raw_read(is, of.ColorPlanes_);
	raw_read(is, of.BytesPerLine_);
	byte_letti += (sizeof(of.manifacturer_) + sizeof(of.version_) + sizeof(of.encoding_) + sizeof(of.bitsperplane_) +
		sizeof(of.WindowXmax_) * 4 + sizeof(of.ColorPlanes_) + sizeof(of.BytesPerLine_));
	is.ignore(128-byte_letti);

	int rows = of.WindowYmax_ - of.WindowYmin_ + 1;
	int cols = of.WindowXmax_ - of.WindowXmin_ + 1;
	img.resize(rows, cols);
	return true;
}

bool load_pcx(const std::string& filename, mat<uint8_t>& img) {
	std::ifstream is(filename, std::ios::binary);
	pcx of;

	if (!is) {
		std::cerr << "file non esistente";
		return false;
	}
	if (!parse_header(is,img,of)) {
		std::cerr << "unable to parse header";
		return false;
	}
	size_t numbytes = of.BytesPerLine_;
	size_t count = 0;
	uint8_t head;
	uint8_t val;
	uint8_t numero_di_byte_da_scrivere;
	int linee = 0;
	std::vector<uint8_t> buffer;
	while (1)
	{
		raw_read(is, head);
		if (!is) {
			break;
		}
		if (((head>>6)&0b11) == 3)
		{
			numero_di_byte_da_scrivere = head & ((1 << 6) - 1);
			raw_read(is, val);
			buffer.insert(buffer.end(), numero_di_byte_da_scrivere, val);
			count += numero_di_byte_da_scrivere;
		}
		else
		{
			val = head;
			numero_di_byte_da_scrivere = 1;
			buffer.insert(buffer.end(), numero_di_byte_da_scrivere, val);
			count += numero_di_byte_da_scrivere;
		}
		
		if (count>=numbytes)
		{
			count = 0;
			for (int i = 0; i < img.cols()/8; i++) {
				val=buffer[i/8];
				for (size_t j = 0; j < 8; j++)
				{
					 val= (val >> (7 - j)) & 1;
					 if (val) {
						 val = 255;
					 }
					 img(linee, i + j)=val;

				}
			}
			linee++;
			buffer.clear();
		}
		if (!is) {
			break;
		}
	}


	return true;
}