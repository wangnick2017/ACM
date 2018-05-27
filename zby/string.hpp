#ifndef tstring_hpp
#define tstring_hpp

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;

template <int size>
struct tstring {

	char m[size];
	int len = 0;

	tstring() {
		len = 0;
	}

	tstring(int t) {
		if (t == 0) {
			len = 0;			
		}
		else{
			len = size;
			for(int i = 0; i < size; i++) m[i] = 127;
		}
	}

	tstring (const char *b) {
		len = min(size, (int)strlen(b));
		for(int i = 0; i < len; i++) m[i] = b[i];
	}

	tstring (char *b) {
		len = min(size, (int)strlen(b));
		for(int i = 0; i < len; i++) m[i] = b[i];
	}

	tstring(string &b) {
		len = min(size, (int)b.length());
		for(int i = 0; i < len; i++) m[i] = b[i];
	}

	tstring<size> &operator= (const string &b) {
		len = min(size, (int)b.length());
		for(int i = 0; i < len; i++) m[i] = b[i];
		return (*this);
	}

	int getLen() const {
		return len;
	}

	int stoi() {
		int ret = 0;
		for(int i = len - 1; i >= 0; i--) ret = ret * 10 + m[i] - '0';
		return ret;
	}

	void print() {
		for(int i = 0; i < len; i++) cout << m[i];
	}

	template <int s2>
	tstring(tstring<s2> &b) {
		len = min(b.getLen(), size);
		for(int i = 0; i < len; i++) m[i] = b[i];
	}

	operator string () const {
		string ret = "";
		for(int i = 0; i < len; i++) ret += m[i];
			return ret;
	}

	char operator [] (int pos) const {
		if (pos > len) return '\0';
		return m[pos];
	}

	template <int s2>
	tstring<size> &operator += (tstring<s2> &rhs) {
		int tr = len + rhs.len;
		if (size < tr) tr = size;
		for(int i = len; i < tr; i++) m[i] = rhs.m[i - len];
		len = tr;
		return (*this);
	}

	tstring<size> &operator += (char &rhs) {
		if (len == size) return (*this);
		len += 1;
		m[len - 1] = rhs;
		return (*this);
	}

	tstring<size> &operator += (const string &rhs) {
		int tr = len + rhs.length();
		if (size < tr) tr = size;
		for(int i = len; i < tr; i++) m[i] = rhs[i - len];
		len = tr;
		return (*this);
	}

	tstring<size> &operator += (char *rhs) {
		int tr = len + strlen(rhs);
		if (size < tr) tr = size;
		for(int i = len; i < tr; i++) m[i] = rhs[i - len];
		len = tr;
		return (*this);
	}

	template <int s2>
	bool operator == (tstring<s2> &b) {
		if (b.getLen() != len) return false;
		for(int i = 0; i < len; i++) {
			if (m[i] != b[i]) return false;
		}
		return true;
	}

	template <int s2>
	bool operator != (tstring<s2> &b) {
		if (b.getLen() != len) return true;
		for(int i = 0; i < len; i++) {
			if (m[i] != b[i]) return true;
		}
		return false;
	}

	template <int s2>
	bool operator < (const tstring<s2> &b) {
		int tlen = min(len, b.getLen());
		for(int i = 0; i < tlen; i++) {
			if (m[i] < b[i]) return true;
			if (m[i] > b[i]) return false;
		}
		if (len < b.getLen()) return true;
		return  false;
	}

};

class cmp{
public:

	template <int s1, int s2>
	bool operator() (const tstring<s1> &a, const tstring<s2> &b){
		int tlen = min(a.getLen(), b.getLen());
		for(int i = 0; i < tlen; i++) {
			if (a[i] < b[i]) return true;
			if (a[i] > b[i]) return false;
		}
		if (a.getLen() < b.getLen()) return true;
		return  false;
	}

};

string fill_to(string b, int tar) {
	string ret = b;
	while (ret.length() < tar) ret = '0' + ret;
	return ret;
}

#endif