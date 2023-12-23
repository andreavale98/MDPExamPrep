#include<string>
#include<fstream>
#include<vector>
#include <iostream>
#include <algorithm>

struct PAM {
	int WIDTH;
	int HEIGHT;
	int DEPTH;
	int MAXVAL ;
	std::string TUPLTYPE;
	std::vector<char> pixels;


	PAM(int width, int height, int depth, std::string tupltype, std::vector <char> v) {
		WIDTH = width;
		HEIGHT = height;
		DEPTH = depth;
		MAXVAL = 255;
		TUPLTYPE = tupltype;
		pixels = v;

	}
};
class PAMwriter {
	std::ofstream of;
public:
	PAMwriter(std::string filename) {
		of = std::ofstream(filename,std::ios::binary);
	}
	void create_example_image(const int height, const int width) {
		std::vector<char> pixels(height*width);
		int val = 0;
		for (size_t i = 0; i < height*width; i++)
		{
			val = i / width;
			pixels[i]=val;

		}
		PAM out = PAM(width, height, 1, "GREYSCALE", pixels);
		print_image(out);


	}
	void print_image(PAM image) {
		of << "P7\n" << "WIDTH " << image.WIDTH << "\n";
		of << "HEIGHT " << image.HEIGHT << "\n";
		of << "DEPTH " << image.DEPTH << "\n";
		of << "MAXVAL " << image.MAXVAL << "\n";
		of << "TUPLTYPE " << image.TUPLTYPE<< "\n";
		of << "ENDHDR" << "\n";
		of.write(&image.pixels[0], image.pixels.size());
	}

};

int main() {

	PAMwriter prova("prova.pam");
	prova.create_example_image(256,256);
	return 0;
}