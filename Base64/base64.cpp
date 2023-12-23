#include "base64.h"
#include <bitset>
#include <sstream>
#include<iostream>
#include <vector>

char ascii_to_base_alphabet(char c) {
	if (c >= 'a' && c <= 'z') {
		return c - 'a' + 26;
	}
	else if (c >= 'A' && c <= 'Z') {
		return c - 'A';
	}
	else if (c >= '0' && c <= '9')
	{
		return c - '0' + 52;
	}
	else if (c == '+') {
		return 62;
	}
	else if (c == '/')
	{
		return 63;
	}
	else if (c == '=') {
		return 0;
	}
	else
	{
		return -1;
	}
}
std::string base64_decode(const std::string& input) {
	if (input.size() <= 0)
	{
		return "";
	}
	std::bitset<24> b;
	std::bitset<24> mask(0xFF);
	std::string out;
	std::vector<char> v;
	char c1, c2, c3, c4;
	size_t cur=0;
	if (input.size() > 4) {
		for (size_t i = 0; i < input.size()- 4; i++)
		{
			c1 = ascii_to_base_alphabet(input[i]);

			if (c1 >= 0) {
				v.push_back(c1);
			}
			if (v.size() == 4)
			{
				c1 = v[0];
				c2 = v[1];
				c3 = v[2];
				c4 = v[3];
				cur = (c1 << 18) + (c2 << 12) + (c3 << 6) + c4;
				b = cur;
				c1 = ((b >> 16) & mask).to_ulong();
				c2 = ((b >> 8) & mask).to_ulong();
				c3 = (b & mask).to_ulong();
				out.append({ c1,c2,c3 });
				v.clear();
			}

		}
	}
	cur = input.size() - 4;
	c1 = ascii_to_base_alphabet(input[cur]);
	c2 = ascii_to_base_alphabet(input[cur + 1]);
	c3 = ascii_to_base_alphabet(input[cur + 2]);
	c4 = ascii_to_base_alphabet(input[cur + 3]);
	cur = (c1 << 18) + (c2 << 12) + (c3 << 6) + c4;
	b = cur;
	c1 = ((b >> 16) & mask).to_ulong();
	c2 = ((b >> 8) & mask).to_ulong();
	c3 = (b & mask).to_ulong();
	if (input[input.size() - 2] == '=')
	{
		out.append({ c1 });
	}
	else if (input[input.size() - 1] == '=') {
		out.append({ c1,c2 });
	}
	else
	{
		out.append({ c1,c2,c3 });
	}


	return out;
}
