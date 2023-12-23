#include <utility>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include <algorithm>
#include <iterator>
#include<inttypes.h>
#include<map>
#include<bitset>
#include<format>
#include <iomanip>
#include <unordered_map>


constexpr auto SCRIVI = false;

template<typename T>
std::ostream& raw_write(std::ofstream& os, T& val,size_t size=sizeof(T)) {
    return os.write(reinterpret_cast<const char*>(&val), size);

}
template<typename T>
std::istream& raw_read(std::ifstream& os, T& val, size_t size = sizeof(T)) {
    return os.read(reinterpret_cast<char*>(&val), size);

}
void scrivi_file() {
	//std::ofstream output("myfile.bin", std::ios::binary);
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine{ rnd_device() };  // Generates random integers
    std::uniform_int_distribution<int> dist{ 0,255 };

    auto gen = [&dist, &mersenne_engine]() {
        return dist(mersenne_engine);
    };

    std::vector<uint8_t> vec(100000);
    std::generate(begin(vec), end(vec), gen);


    std::ofstream output("myfile.bin", std::ios::binary);
    if (!output)
    {
        return;
    }
    for (auto& x : vec) {
        output << x << '\t';
    }
}

void esercizio_1(const char* inputfile, const char* outputfile) {
    std::map<unsigned char, int> m;
    std::ifstream input(inputfile, std::ios::binary);
    if (!input) {
        std::cout << "errore";
        exit(1);
    }
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    for (const auto& x : buffer) {
        m[x] += 1;
    }

    std::ofstream os(outputfile);
    for (const auto& p : m)
    {
        /*os << std::hex << std::setfill('0') << std::setw(2) << +p.first;
    os << '\t' << std::dec<<p.second << std::endl;*/
        os << std::format("{:02X}\t{}\n", p.first, p.second);
    }
}
void conta_parole(const char* infile, const char* outfile) {
    std::map<std::string, unsigned int> m;
    std::ifstream input(infile, std::ios::binary);
    if (!input) {
        std::cout << "errore";
        exit(1);
    }
    std::string buf;
    while (input >> buf)
    {
        m[buf] += 1;

    }
    std::ofstream os(outfile);
    for (const auto& p : m) {
        os << p.first <<'\t'<< p.second<<'\n';
    }
}
void esercizio_2(const char* infile, const char* outfile){
    std::ifstream input(infile);
    if (!input) {
        std::cout << "errore";
        exit(1);
    }
    
    std::istream_iterator<int> start(input);
    std::istream_iterator<int> stop;
    std::vector<int> v(start, stop);
    std::ofstream os(outfile,std::ios::binary);
    for (auto &x:v)
    {
        raw_write(os, x);
        os << "\n";
    }
    }

void esercizio_3(const char* infile, const char* outfile) {
    std::ifstream is(infile);
    char buffer[4];
    int val;
    std::ofstream os(outfile);
    std::vector<int>v;
    while (raw_read(is,val))
    {
        v.push_back(val);

    }

    std::copy(v.begin(), v.end(),
        std::ostream_iterator<int>(os, "\n"));

}

void estrai_bit(bool *b,int n) {
    for (size_t i = 0; i < 11; i++)
    {
        b[i] = (n >> i)&1;
    }
}

void esercizio_4(const char* infile, const char* outfile) {
    /*
    leggere in binario
    for (bit in numero)
    shift>>i
    num&1

    scrivere in binario
    for(bits in buffer)
    buffer*2+1
    */

    std::ifstream is(infile);
    std::ofstream os(outfile);
    size_t posizione_bitset=0;
    char buf=0;
    char bit_scritti = 0;
    bool b[11];
    std::istream_iterator<int> start(is);
    std::istream_iterator<int> stop;
    std::vector<int> nums(start, stop);
    for (auto& n : nums) {
        estrai_bit(b,n);
        while (posizione_bitset<11) 
        {
            buf = 2 * buf + b[10-posizione_bitset];
            bit_scritti++;
            posizione_bitset++;
            if (bit_scritti >= 8) {
                os.write(&buf, 1);
                bit_scritti = 0;
                buf = 0;
            }
        }
        
        if (posizione_bitset >= 11) {
            posizione_bitset = 0;
            continue;
        }
    }
}
int main() {
	if (SCRIVI) {
        scrivi_file();
	}
    //esercizio_1("C:/Users/Andrea/Documents/uni/Magistrale/Multimedia data processing/bibbia.txt", "occurrences.txt");
    //esercizio_2("file02.txt", "es02.bin");
    //esercizio_3("es02.bin", "es03.txt");
    esercizio_4("file02.txt", "es04.bin");

}