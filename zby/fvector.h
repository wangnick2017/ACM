#ifndef FVECTOR_HPP
#define FVECTOR_HPP
#include <fstream>
#include "string.h"
using namespace std;
template<class T>
class fvector
{
private:

	stl::string<20> fileName;
	fstream f;

public:

	void init(const char *_fileName)
	{
		fileName = _fileName;
		f.open(fileName + ".bin", fstream::in);
		if (!f) {
			f.open(fileName + ".bin", fstream::out | fstream::trunc | fstream::binary);
			int tot = 0;
			f.write(reinterpret_cast<char*>(&tot), sizeof(int));
		}
		f.close();
	}

	int size()
	{
		f.open(fileName + ".bin", fstream::in | fstream::binary);
		f.seekg(0, ios::beg);
		int tot;
		f.read(reinterpret_cast<char*>(&tot), sizeof(int));
		f.close();
		return tot;
	}

	void push_back(const T &x)
	{
		f.open(fileName + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		f.seekg(0, ios::beg);
		f.read(reinterpret_cast<char*>(&tot), sizeof(int));
		f.seekp(sizeof(int) + sizeof(T) * tot, ios::beg);
		T tmp = x;
		f.write(reinterpret_cast<char*>(&tmp), sizeof(T));
		f.seekp(0, ios::beg);
		++tot;
		f.write(reinterpret_cast<char*>(&tot), sizeof(int));
		f.close();
	}

	T operator[](int pos)
	{
		f.open(fileName + ".bin", fstream::in | fstream::binary);
		T tmp;
		f.seekg(sizeof(int) + sizeof(T) * pos, ios::beg);
		f.read(reinterpret_cast<char*>(&tmp), sizeof(T));
		f.close();
		return tmp;
	}

	void modify(int pos, const T &x)
	{
		f.open(fileName + ".bin", fstream::out | fstream::binary | fstream::in);
		f.seekp(sizeof(int) + sizeof(T) * pos, ios::beg);
		T tmp = x;
		f.write(reinterpret_cast<char*>(&tmp), sizeof(T));
		f.close();
	}
};
#endif