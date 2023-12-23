#include <iostream>
#include <fstream>
#include <vector>


class bitreader {
    std::istream &is_;
    uint8_t buf_;
    uint8_t len_;
public:
    bitreader(std::istream &is) : is_(is), len_(0) {

    }

    uint8_t read_bit() {
        if(len_ > 0) {
            uint8_t ret = (buf_ >> (len_-1)) & 1;
            len_--;
            return ret;
        } else {
            len_ = 8;
            is_.read((char *) &buf_, 1);
            return read_bit();
        }
    }

    uint64_t read_bits(uint8_t size) {
        uint64_t ret = 0;
        do {
            ret += read_bit() << size-1;
        } while(--size);

        return ret;
    }
};


size_t read_offset(bitreader &br) {
    uint8_t first_bit = br.read_bit();

    if(first_bit == 1) {
        return br.read_bits(7);
    } else {
        return br.read_bits(11);
    }
}

size_t read_length(bitreader &br) {
    uint8_t first_bits = br.read_bits(2);

    if(first_bits != 0b11) {
        return first_bits+2;
    } else {
        uint8_t second_bits = br.read_bits(2);
        if(second_bits != 0b11) {
            return second_bits+5;
        } else {
            size_t N = 0;
            uint8_t read_bits;
            do {
                read_bits = br.read_bits(4);
                N++;
            } while(read_bits == 0xF);
            return read_bits + (N*15 - 7);
        }
    }

}

void lzs_decompress(std::istream& is, std::ostream& os) {
    int iter = 0;
    bitreader br(is);
    std::vector<uint8_t> hist;
    while(1) {
        auto buf = br.read_bit();
        if(!is) {
            puts("Early return");
            break;
        }
        if(buf == 0) {
            uint8_t byte = br.read_bits(8);
            if(!is) {
                puts("Early return");
                break;
            }
            hist.push_back(byte);
        } else {
            size_t offset = read_offset(br);
            if(offset == 0) break;
            size_t length = read_length(br);
            size_t start = hist.size()-offset;
            for(size_t i = 0; i < length; i++) {
                hist.push_back(hist[start+i]);
            }
        }
        iter++;
    
    }

    os.write((char *) hist.data(), hist.size());
}