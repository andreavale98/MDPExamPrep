#include "Bitreader.cpp"
#include "Bitwriter.cpp"
#include <iostream>
using namespace std;

int elias_encode(char* filein, char* fileout) {
	ifstream is = std::ifstream(filein);
	if (!is) {
		cout << "errore all'apertura del file";
		return 1;
	}
	ofstream os = ofstream(fileout, ios::binary);
	bitwriter bw = bitwriter(os);
	std::istream_iterator<int> start(is);
	std::istream_iterator<int> stop;
	std::vector<int> v(start, stop);
	vector<unsigned int> nums;
	for (size_t i = 0; i < v.size(); i++)
	{
		if (v[i] <0) {
			unsigned int num = -v[i] * 2;
			nums.push_back(num);
		}
		else
		{
			unsigned int num = v[i] * 2 + 1;
			nums.push_back(num);
		}
	}
	//auto max = max_element(nums.begin(), nums.end());
	for (auto &num:nums)
	{
		int bits = log2(num);
		bw(0,bits);
		bw(num, bits + 1);
	}

	return 0;
}

int elias_decode(char* filein, char* fileout) {
	ifstream is = std::ifstream(filein,ios::binary);
	if (!is) {
		cout << "errore all'apertura del file";
		return 1;
	}
	ofstream os = ofstream(fileout);
	bitreader br = bitreader(is);
	vector <int> v;
	int nbit = 0;
	int num = 0;
	while (1)
	{
		int bit = br(1);
		if (!br) {
			break;
		}
		while (bit==0)
		{
			nbit++;
			bit = br(1);
		}
		num = br(nbit);
		if (br)
		{
			v.push_back((1 << nbit) + num);
			nbit = 0;

		}
		
		
	}
	vector<int> nums;
	for (size_t i = 0; i < v.size(); i++)
	{
		if (v[i] %2==0) {
			num = -v[i] / 2;
			nums.push_back(num);
		}
		else
		{
			num = (v[i]-1)/2;
			nums.push_back(num);
		}
	}
	std::copy(nums.begin(), nums.end(),
		std::ostream_iterator<int>(os, "\n"));

	return 0;
}


int main(int argc, char** argv) {
	if (argc != 4) {
		std::cout << "Usage: elias [c|d] <filein> <fileout>";
	}
	int out = 0;
	if (strcmp(argv[1],"c")==0)
	{
		 out=elias_encode(argv[2], argv[3]);
	}
	else if (strcmp(argv[1],"d")==0)
	{
		out=elias_decode(argv[2],argv[3]);
	}
	return out;
}