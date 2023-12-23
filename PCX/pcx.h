#pragma once
#include "mat.h"
#include "types.h"
#include "utils.h"
#include "pgm.h"
#include <string>
#include <fstream>
bool load_pcx(const std::string& filename, mat<uint8_t>& img);
struct pcx {
	uint8_t manifacturer_;
	uint8_t version_;
	uint8_t encoding_;
	uint8_t bitsperplane_;
	uint16_t WindowXmin_;
	uint16_t WindowXmax_;
	uint16_t WindowYmin_;
	uint16_t WindowYmax_;
	uint8_t ColorPlanes_;
	uint16_t BytesPerLine_;

	pcx() {}
};