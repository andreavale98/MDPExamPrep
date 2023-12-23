#include "pbm.h"



bool BinaryImage::ReadFromPBM(const std::string& filename) {
	std::ifstream is(filename, std::ios::binary);
	std::string magic, width, height;
	uint8_t buf;
	is >> magic;
	uint8_t mask;
	if (!is)
	{
		std::cerr << "errore nell'aprire il file";
		return false;
	}
	if (magic != "P4") {
		std::cerr << "file non pbm";
		return false;
	}
	is.get();
	if (is.peek() == '#') {
		while (is.get() != '\n')
		{

		}
	}
	is >> width;
	W = std::stoi(width);
	is.get();
	is >> height;
	H = std::stoi(height);
	is.get();

	if (H < 0 || W < 0) {
		std::cerr << "errore nel leggere width o height";
		return false;
	}

	for (int i = 0; i < H; i++)
	{
		/*
		for (int j = 0; j < W / 8; j++)
		{
			is.read((char*)&buf, 1);
			ImageData.push_back(buf);

		}
		if (W%8!=0)
		{
			is.read((char*)&buf, 1);
			buf = (buf >> (8 - (W % 8))) & mask;
			buf = (buf << (8 - (W % 8)));
			ImageData.push_back(buf);
		}*/
		for (int j = 0; j < (W / 8) + 1; j++)
		{
			is.read((char*)&buf, 1);
			ImageData.push_back(buf);
		}
	}

	return true;
};
BinaryImage::BinaryImage() {
	W = 0;
	H = 0;
}
BinaryImage::BinaryImage(const std::string& filename) {
	ReadFromPBM(filename);
};

Image BinaryImageToImage(const BinaryImage& bimg) {
	Image out;
	int H = bimg.H;
	int W = bimg.W;
	out.H = H;
	out.W = W;
	uint8_t buf;
	bool bit;
	uint8_t mask = (1 << (W % 8)) - 1;
	int npixel = 0;

	if (H < 0 || W < 0) {
		std::cerr << "errore nel leggere width o height";
		return out;
	}

	for (int i = 0; i < H; i++)
	{

		for (int j = 0; j < (W / 8); j++)
		{
			buf = bimg.ImageData[i * (W / 8 + 1) + j];
			for (int h = 0; h < 8; h++)
			{
				bit = (buf >> (7 - h)) & 1;
				if (bit) //bit 1 -> nero->0
				{
					out.ImageData.push_back(0);
					npixel++;
				}
				else
				{
					out.ImageData.push_back(255);
					npixel++;
				}
			}

		}
		if (W % 8 != 0)
		{
			buf = bimg.ImageData[(i+1) * (W / 8)];
			buf = (buf >> (8 - (W % 8))) & mask;
			buf = (buf << (8 - (W % 8)));

			for (int h = 0; h < (W % 8); h++)
			{
				bit = (buf >> (7 - h)) & 1;
				if (bit) //bit 1 -> nero->0
				{
					out.ImageData.push_back(0);
					npixel++;
				}
				else
				{
					out.ImageData.push_back(255);
					npixel++;
				}
			}
	}


}
return out;
}