/*
	database接口
	D为数据类型，K为索引类型
*/
#ifndef DB_HPP
#define DB_HPP

#include <iostream>
#include <fstream>
#include "string.h"
#include "pair.h"
using namespace std;
template<class D, class K>
class DB{
private:
	stl::string<30> fileName;
	int keyNum;
public:
	struct node
	{
		D Data;
		K Key[5];
		bool exist;
	};
	void init(const char *filename, int key_num)
	{
		/*
			初始化数据库，如果没有对应文件则新建
			key_num为索引个数
		*/
		fileName = filename;
		keyNum = key_num;
		fstream bin;
		stl::string<30> s = filename;
		bin.open(s + ".bin", fstream::in);
		if (!bin) {
			bin.open(s + ".bin", fstream::out | fstream::binary | fstream::trunc);
			int tot = 0;
			bin.write(reinterpret_cast<char*>(&tot), sizeof(int));
		}
		bin.close();
	}
	
	int insert(D data, const K *key) 
	{
		/*
			插入一个数据，数据为data，key为包含key_num个索引的数组，其中key[0]为主key（保证唯一）
		*/
		fstream bin;
		stl::string<30> s = fileName;
		bin.open(s + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(reinterpret_cast<char*>(&tot), sizeof(int));
		node tmp;
		int pos = tot;
		for (int i = 0; i < tot; ++i) {
			bin.read(reinterpret_cast<char*>(&tmp), sizeof(node));
			if (!tmp.exist) {
				/*tmp.exist = true;
				tmp.Data = data;
				for (int j = 0; j < keyNum; ++j)
					tmp.Key[j] = key[j];
				bin.seekp(sizeof(int) + sizeof(node) * i, ios::beg);
				bin.write(reinterpret_cast<char*>(&tmp), sizeof(node));
				bin.close();
				return 1;*/
				pos = i;
			}
			if (tmp.exist && tmp.Key[0] == key[0]) {
				bin.close();
				return 0;
			}
		}
		if (pos == tot) {
			++tot;
			bin.seekp(0, ios::beg);
			bin.write(reinterpret_cast<char*>(&tot), sizeof(int));
		}
		tmp.exist = true;
		tmp.Data = data;
		for (int j = 0; j < keyNum; ++j)
			tmp.Key[j] = key[j];
		bin.seekp(sizeof(int) + sizeof(node) * pos, ios::beg);
		bin.write(reinterpret_cast<char*>(&tmp), sizeof(node));
		bin.close();
		return 1;
	}

	int modify(K key, D n_data, const K *n_key)
	{
		/*
			修改，key为主索引，n_data和n_key为修改后data和key
		*/
		fstream bin;
		stl::string<30> s = fileName;
		bin.open(s + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(reinterpret_cast<char*>(&tot), sizeof(int));
		node tmp;
		for (int i = 0; i < tot; ++i) {
			bin.read(reinterpret_cast<char*>(&tmp), sizeof(node));
			if (tmp.Key[0] == key && tmp.exist) {
				tmp.Data = n_data;
				for (int j = 0; j < keyNum; ++j)
					tmp.Key[j] = n_key[j];
				bin.seekp(sizeof(int) + sizeof(node) * i, ios::beg);
				bin.write(reinterpret_cast<char*>(&tmp), sizeof(node));
				bin.close();
				return 1;
			}
		}
		bin.close();
		return 0;
	}

	stl::pair<D*, bool> find_main(K key) 
	{
		/*
			查询主索引为key的数据
			未找到则返回(NULL, false)
			成功为(指向目标data的指针, true)
		*/
		fstream bin;
		stl::string<30> s = fileName;
		bin.open(s + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(reinterpret_cast<char*>(&tot), sizeof(int));
		node tmp;
		for (int i = 0; i < tot; ++i) {
			bin.read(reinterpret_cast<char*>(&tmp), sizeof(node));
			if (tmp.Key[0] == key && tmp.exist) {
				D *t = new D(tmp.Data);
				bin.close();
				return stl::pair<D*, bool>(t, true);
			}
		}
		bin.close();
		return stl::pair<D*, bool>(NULL, false);
	}

	stl::pair<D*, int> find_sub(K key, int pos) 
	{
		/*
			查询满足索引数组中下标为pos处值为key的所有data
			返回数组和满足条件的个数
		*/
		fstream bin;
		stl::string<30> s = fileName;
		bin.open(s + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(reinterpret_cast<char*>(&tot), sizeof(int));
		node tmp;
		int cnt = 0;
		for (int i = 0; i < tot; ++i) {
			bin.read(reinterpret_cast<char*>(&tmp), sizeof(node));
			if (tmp.Key[pos] == key && tmp.exist)
				++cnt;
		}
		D *t = new D[cnt];
		int j = 0;
		bin.seekg(sizeof(int), ios::beg);
		for (int i = 0; i < tot; ++i) {
			bin.read(reinterpret_cast<char*>(&tmp), sizeof(node));
			if (tmp.Key[pos] == key && tmp.exist)
				t[j++] = tmp.Data;
			if (j == cnt)
				break;
		}
		bin.close();
		return stl::pair<D*, int>(t, cnt);
	}

	void del(K key) 
	{
		/*
			删除主索引为key的数据
		*/	
		fstream bin;
		stl::string<30> s = fileName;
		bin.open(s + ".bin", fstream::in | fstream::out | fstream::binary);
		int tot;
		bin.read(reinterpret_cast<char*>(&tot), sizeof(int));
		node tmp;
		for (int i = 0; i < tot; ++i) {
			bin.read(reinterpret_cast<char*>(&tmp), sizeof(node));
			if (tmp.Key[0] == key) {
				tmp.exist = false;
				bin.seekp(sizeof(int) + sizeof(node) * i, ios::beg);
				bin.write(reinterpret_cast<char*>(&tmp), sizeof(node));
				bin.close();
				return;
			}
		}
		bin.close();
	}

};

#endif