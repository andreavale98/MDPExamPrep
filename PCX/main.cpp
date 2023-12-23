#include "pcx.h"
int main() {
	mat<uint8_t> img;
	load_pcx("bunny.pcx",img);
	save_pgm("diocane.pgm", img);
	return 0;
}