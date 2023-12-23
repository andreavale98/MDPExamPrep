#include "pbm.h"
int main() {
	BinaryImage bi;
	bi.ReadFromPBM("im1.pbm");
/*
	std::ofstream os("prova.bin", std::ios::binary);
	os.write((char*)bi.ImageData.data(), bi.ImageData.size());
*/
	BinaryImageToImage(bi);

}