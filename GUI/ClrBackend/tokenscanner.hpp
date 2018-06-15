#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "string.hpp"
#include <string>
#include <iostream>


class scanner {
	char *buffer;

public:
	scanner() {
		buffer = NULL;
	}

	void set(char *in) {
		buffer = in;
	}

	/*template<int l>
	tstring<l> getts() {
		while (buffer[0] == ' ' || buffer[0] == '\n') buffer++;
		tstring<l> ret;
		while (buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\0') {
			ret += buffer[0];
			buffer++;
		}
		return ret;
	}*/

	std::string gets() {
		if (buffer == NULL) {
            std::string ret;
            std::cin >> ret;
			return ret;
		}
		while (buffer[0] == ' ' || buffer[0] == '\n') buffer++;
        std::string ret;
		while (buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\0') {
			ret += buffer[0];
			buffer++;
		}
		return ret;
	}

	int geti() {
		if (buffer == NULL) {
			int ret;
            std::cin >> ret;
			return ret;
		}
		while (buffer[0] == ' ' || buffer[0] == '\n') buffer++;
		int ret = 0;
		while (buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\0') {
			ret = ret * 10 + buffer[0] - '0';
			buffer++;
		}
		return ret;
	}

	double getd() {
		if (buffer == NULL) {
			double ret;
            std::cin >> ret;
			return ret;
		}
		while (buffer[0] == ' ' || buffer[0] == '\n') buffer++;
		double ret = 0, w = 0;
		while (buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\0') {
			if (buffer[0] == '.') w = 1;
			else if (buffer[0] < '0' || buffer[0] > '9') ret = 0, w = 0;
			else ret = ret * 10 + buffer[0] - '0', w *= 10;
			buffer++;
		}
		ret /= w;
		return ret;	
	}

	char getc() {
		if (buffer == NULL) {
			char ret = getchar();
			while (ret == ' ' || ret == '\n' || ret == '\0') ret = getchar();
			return ret;
		}
		while (buffer[0] == ' ' || buffer[0] == '\n') buffer++;
		char ret = buffer[0]; buffer++;
		return ret;
	}
};

#endif