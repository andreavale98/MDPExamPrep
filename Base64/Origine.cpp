#include"base64.h"
#include<iostream>

int main() {
	std::string s = "YWJj\n";
	s = base64_decode(s);
	std::cout << s;
}