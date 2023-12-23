#ifndef PGM_H
#define PGM_H
#include <string>
#include "mat.h"

bool load(const std::string& filename, mat<uint16_t>& img, uint16_t& maxvalue);
#endif // !PGM_H
