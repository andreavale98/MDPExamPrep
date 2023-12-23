#include<cstring>
#include<cstdlib>
#include <string>
#include<iostream>
#include <fstream>
#include <vector>
#include <map>
#include<algorithm>

bool myfunction(float i, float j) { return (i > j); }



template<typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T)) {
	return os.write(reinterpret_cast<const char*>(&val), size);
}

class bitwriter {
	uint8_t buffer_=0;
	int n_ = 0;
	std::ostream& os_;

	std::ostream& write_bit(uint32_t bit) {
		buffer_ = (buffer_ << 1) | (bit & 1);
		++n_;
		if (n_ == 8) {
			raw_write(os_, buffer_);
			n_ = 0;
		}
		return os_;
	}

public:
	bitwriter(std::ostream& os) : os_(os) {}

	std::ostream& write(uint32_t u, uint8_t n) {
		//while (n --> 0) {
		//  write_bit(u >> n);
		//}
		for (int i = n - 1; i >= 0; --i) {
			write_bit(u >> i);
		}
		return os_;
	}

	std::ostream& operator()(uint32_t u, uint8_t n) {
		return write(u, n);
	}

	std::ostream& flush(uint32_t bit = 0) {
		while (n_ > 0) {
			write_bit(bit);
		}
		return os_;
	}

	~bitwriter() {
		flush();
	}
};

template<typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T)) {
	return is.read(reinterpret_cast<char*>(&val), size);
}

class bitreader {
	uint8_t buffer_=0;
	uint8_t n_ = 0;
	std::istream& is_;

public:
	bitreader(std::istream& is) : is_(is) {}

	uint32_t read_bit() {
		if (n_ == 0) {
			raw_read(is_, buffer_);
			n_ = 8;
		}
		--n_;
		return (buffer_ >> n_) & 1;
	}

	uint32_t read(uint8_t n) {
		uint32_t u = 0;
		while (n-- > 0) {
			u = (u << 1) | read_bit();
		}
		return u;
	}

	uint32_t operator()(uint8_t n) {
		return read(n);
	}

	bool fail() const {
		return is_.fail();
	}

	explicit operator bool() const {
		return !fail();
	}
};

struct huffman_node {
	huffman_node* left_;
	huffman_node* right_;
	std::vector<unsigned char> symbols_;
	float frequence_;
	huffman_node(huffman_node* left, huffman_node* right, std::vector<unsigned char> symbols, float frequence) {
		left_ = left;
		right_ = right;
		symbols_ = symbols;
		frequence_ = frequence;
	}
	huffman_node(std::vector<unsigned char> symbols, float frequence) {
		left_ = NULL;
		right_ = NULL;
		symbols_ = symbols;
		frequence_ = frequence;
	}
};
bool cmp(const huffman_node& a, const huffman_node& b) {
	return a.frequence_ < b.frequence_;
}
struct huffmanTree
{
	std::vector<huffman_node> tree_;
	huffmanTree() {

	}

	

	huffmanTree(std::map<unsigned char, float> freq) {
		for (auto &x : freq)
		{
			std::vector<unsigned char> v = {x.first};
			tree_.push_back(huffman_node(v, x.second));
		}
		this->build_tree();
	}
	void create_father (huffman_node &a, huffman_node &b){
		std::vector<unsigned char> symbols = a.symbols_;
		symbols.insert(symbols.end(), b.symbols_.begin(), b.symbols_.end());
		huffman_node father(&a, &b, symbols, a.frequence_ + b.frequence_);
		this->tree_.erase(this->tree_.begin(), this->tree_.begin() + 2); // cancella i primi 2 elementi
		this->tree_.push_back(father);

	}
	void build_tree() {
		while (this->tree_.size()>1)
		{
			this->sort_tree();
			this->create_father(this->tree_[0],this->tree_[1]);
		}
		
		
	}

	void sort_tree() {
		std::sort(this->tree_.begin(), this->tree_.end(), cmp);
		
	}
};


void huffmandecode(char* fin, char* fout) {

}

void huffmanencode(char* fin, char*fout) {
	std::ifstream input(fin, std::ios::binary);
	if (!input) {
		std::cout << "errore";
		exit(1);
	}
	std::vector<unsigned char> in(std::istreambuf_iterator<char>(input), {});
	std::map<unsigned char, float> m;
	float num_items = in.size();
	for (const auto& x : in) {
		m[x] += 1.0/num_items;
	}
	huffmanTree Tree=huffmanTree(m);
	std::cout << "diocaner";
	int code = 0;
	std::map<unsigned char, int> codes;
	for (auto& x : m) {
		std::vector<unsigned char>& right = Tree.tree_[0].right_->symbols_;
		std::vector<unsigned char>& left = Tree.tree_[0].left_->symbols_;
		/*
		if (std::find(left.begin(), left.end(), x) != left.end()) {
			code = code < 1 + 0;
		}
		else {
			code = code < 1 + 1;
		}
		*/
		
	}



}

int main(int argc, char**argv) {
	if (argc!=4)
	{
		std::cout << "usage: huffman1 [c|d] <input file> <output file>";
	}
	std::string mode = argv[1];

	if (mode=="c")
	{
		huffmanencode(argv[2], argv[3]);
	}
	else if(mode=="d")
	{
		huffmandecode(argv[2], argv[3]);
	}
}