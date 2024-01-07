#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class Bencode {
	std::string type;
	long value=-1;
	std::string string;
	std::vector<Bencode*> v;
	std::map<std::string, Bencode*> m;

	bool readint(std::ifstream& is) { 
		if (is.get() != 'i')
			return false;
		is >> this->value;
		if (is.get()!='e')
		{
			return false;
		}
		return true;


		
	}
	std::string readstring(std::ifstream& is)
	{
		//leggo la lunghezza
		size_t length;
		is >> length;

		//controllo che ci sia :
		if (is.get() != ':') {
			throw std::invalid_argument("Formato non valido: manca il carattere ':'");
		}

		// leggo esattamente n caratteri dallo stream
		std::string string;
		string.resize(length);
		is.read(&string[0], length);

		return string; 
	}
	bool readlist(std::ifstream& is){ 
		if (is.get() != 'l')
			return false;
		while (true)
		{
			Bencode val;
			val.read(is);
			this->v.push_back(&val);
			if (is.peek() == 'e')
			{
				is.get();
				return true;
			}
		}
		return false;
	}
	bool readdic(std::ifstream& is)
	{ 
		if (is.get() != 'd')
			return false;
		while (true)
		{
			std::string key = this->readstring(is);
			Bencode val;
			val.read(is);
			this->m.insert({ key, &val });
			if (is.peek() == 'e')
			{
				is.get();
				return true;
			}
		}
		

		return false; 
	}

public:
	bool read(std::ifstream& is) {
		// dovrei leggere un solo elemento, funzione ricorsiva.
			char type = is.peek(); // guardo il primo carattere (senza leggerlo in caso di stringhe perchè mi serve tutto il numero)
			if (isdigit(type))
				type = '1'; // uso '1' per intercettare tutte le stringhe
			switch (type)
			{
			case 'l': 
				if(!this->readlist(is))
					return false; 
				break;
			case 'd': 
				if (!this->readdic(is))
					return false;
				break;
			case 'i':
				if (!this->readint(is))
					return false;
				break;
			case '1':
				this->type = "string";
				this->string = readstring(is);
				break;
			default: return false;
			}
			return true;
	}
		
};

int main() {
	std::ifstream is("KickAss.torrent");
	Bencode b;
	b.read(is);
}