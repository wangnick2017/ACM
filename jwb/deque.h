#ifndef SJTU_DB_HPP
#define SJTU_DB_HPP
#include <fstream>
#include <functional>
#include "tstring.h"
#include "string.h"
const int max_size = 100;
namespace stl
{
	template<class T>
	class DB
	{
	private:
		tstring<30> dis;
		struct block
		{
			T data[max_size + 1];
			int tot = 0, next = 0, pre = 0;
		};
	public:
		DB(string _dis)
		{
			dis = _dis;
			fstream test(dis + ".bin", fstream::in);
			if (!test) {
				test.open(_dis + ".bin", fstream::trunc | fstream::out | fstream::binary);
				int tot = 1;
				test.write(reinterpret_cast<char*>(&tot), sizeof(int));
				tot = 1;
				test.write(reinterpret_cast<char*>(&tot), sizeof(int));
				block t;
				test.write(reinterpret_cast<char*>(&t), sizeof(block));
				test.close();
			}
			test.close();
		}
		string getdis()
		{
			return dis;
		}
		void insert(T _data)
		{
			fstream bin;
			string _dis = dis;
			bin.open(_dis + ".bin", fstream::in | fstream::out | fstream::binary);
			int tot;
			bin.read(reinterpret_cast<char*>(&tot), sizeof(int));
			int cur;
			bin.read(reinterpret_cast<char*>(&cur), sizeof(int));
			block tb;
			while (cur) {
				bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
				bin.read(reinterpret_cast<char*>(&tb), sizeof(block));
				if (std::less<T>(_data, tb.data[tb.tot - 1]) || tb.next == 0 || _data == tb.data[tb.tot - 1]) {
					for(int i = 0; i <= tb.tot; ++i)
						if (i == tb.tot || std::less<T>(_data, tb.data[i])) {
							for (int j = tb.tot; j > i; --j)
								tb.data[j] = b.data[j - 1];
							tb.data[i] = _data;
							++tb.tot;
							if (tb.tot >= max_size) {
								block qb;
								for (int j = 0; i < max_size / 2; ++j)
									qb.data[j] = tb.data[j + max_size / 2];
								qb.next = tb.next;
								tb.next = tot + 1;
								qb.pre = cur;
								qb.tot = tb.tot = max_size / 2;
								bin.seekp(sizeof(int) * 2 + sizeof(block) * tot, ios::beg);
								bin.write(reinterpret_cast<char*>(&qb), sizeof(block));
								bin.seekp(0, ios::beg);
								++tot;
								bin.write(reinterpret_cast<char*>(&tot), sizeof(int));
							}
							bin.seekp(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
							bin.write(reinterpret_cast<char*>(&tb), sizeof(block));
							bin.close();
							return;
						}
				}
				cur = tb.next;
			}
			bin.close();
		}
		void erase(T _data)
		{
			fstream bin;
			string _dis = dis;
			bin.open(_dis + ".bin", fstream::in | fstream::out | fstream::binary);
			int tot;
			bin.read(reinterpret_cast<char*>(&tot), sizeof(int));
			int cur;
			bin.read(reinterpret_cast<char*>(&cur), sizeof(int));
			block tb;
			while (cur) {
				bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
				bin.read(reinterpret_cast<char*>(&tb), sizeof(block));
				if (std::less<T>(_data, tb.data[tb.tot - 1]) || _data == tb.data[tb.tot - 1]) {
					for(int i = 0; i < tb.tot; ++i)
						if (_data == tb.data[i]) {
							for(int j = i; j < tb.tot; ++j)
								tb.data[j] = tb.data[j + 1];
							--tb.tot;
							if (!tb.tot && tot != 1) {
								if (tb.pre) {
									block qb;
									bin.seekg(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
									bin.read(reinterpret_cast<char*>(&qb), sizeof(block));
									qb.next = tb.next;
									bin.seekp(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
									bin.write(reinterpret_cast<char*>(&qb), sizeof(blcok));
								}
								else {
									int st = tb.next;
									bin.seekp(sizeof(int), ios::beg);
									bin.write(reinterpret_cast<char*>(&st), sizeof(int));
								}
								block qb;
								bin.seekg(sizeof(int) * 2 + sizeof(block) * (tot - 1), ios::beg);
								bin.read(reinterpret_cast<char*>(&qb), sizeof(block));
								memcpy(&tb, &qb, sizeof(block));
								if (tb.pre) {
									qb.next = cur;
									bin.seekp(sizeof(int) * 2 + sizeof(block) * (tb.pre - 1), ios::beg);
									bin.write(reinterpret_cast<char*>(&qb), sizeof(block));
								}
								--tot;
								bin.seekp(0, ios::beg);
								bin.write(reinterpret_cast<char*>(&tot), sizeof(int));
							}
							bin.seekp(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
							bin.write(reinterpret_cast<char*>(&tb), sizeof(block));
							bin.close();
							return;
						}
				}
				cur = tb.next;
			}
			bin.close();
		}
		class iterator
		{
			friend class DB;
		private:
			int cur, pos;
			tstring<30> dis;
		public:
			iterator() = default;
			~iterator() = default;
			iterator(const iterator &other)
			{
				cur = other.cur;
				pos = other.pos;
				dis = other.dis;
			}
			iterator &operator=(const iterator &other)
			{
				cur = other.cur;
				pos = other, pos;
				dis = other.dis;
				return *this;
			}
			iterator &operator++()
			{
				fstream bin;
				string _dis = dis;
				bin.open(_dis + "bin", fstream::in | fstream::out | fstream::bin);
				bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
				block tb;
				bin.read(reinterpret_cast<char*>(&tb), sizeof(block));
				if (pos > tb.tot - 1) {
					cur = tb.next;
					pos = 0;
				}
				else
					++pos;
				bin.close();
				return *this;
			}
			iterator operator++(int)
			{
				iterator tmp = *this;
				fstream bin;
				string _dis = dis;
				bin.open(_dis + "bin", fstream::in | fstream::out | fstream::bin);
				bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
				block tb;
				bin.read(reinterpret_cast<char*>(&tb), sizeof(block));
				if (pos > tb.tot - 1) {
					cur = tb.next;
					pos = 0;
				}
				else
					++pos;
				bin.close();
				return tmp;
			}
			T operator*() const
			{
				fstream bin;
				stirng _dis = dis;
				bin.open(_dis + "bin", fstream::in | fstream::bin);
				bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
				block tb;
				bin.read(reinterpret_cast<char*>(&tb), sizof(int));
				bin.close();
				return tb.data[pos];
			}
			T *operator->() const
			{
				return &(operator*());
			}
		};
	
		iterator begin()
		{
			iterator ret;
			ret.pos = 0;
			fstream bin;
			string _dis = dis;
			ret.dis = dis;
			bin.open(_dis + ".bin", fstream::in | fstream::bin);
			bin.seekg(sizeof(int), ios::beg);
			int tmp;
			bin.read(reinterpret_cast<char*>(&tmp), sizeof(int));
			ret.cur = tmp;
			return ret;
		}
		iterator end()
		{
			iterator ret;
			ret.cur = ret.pos = 0;
			ret.dis = dis;
			return ret;
		}
		iterator find(T _data)
		{
			iterator ret;
			ret.dis = dis;
			fstream bin;
			string _dis = dis;
			bin.open(_dis + ".bin", fstream::in | fstream::bin);
			int tot;
			bin.read(reinterpret_cast<char*>(&tot), sizeof(int));
			int cur;
			bin.read(reinterpret_cast<char*>(&cur), sizeof(int));
			block tb;
			while (cur) {
				bin.seekg(sizeof(int) * 2 + sizeof(block) * (cur - 1), ios::beg);
				bin.read(reinterpret_cast<char*>(&tb), sizeof(block));
				if (std::less<T>(_data, tb.data[tb.tot - 1]) || _data == tb.data[tb.tot - 1]) {
					for (int i = 0; i < tb.tot; ++i)
						if (_data == tb.data[i]) {
							ret.cur = cur;
							ret.pos = i;
							bin.close();
							return ret;
						}
				}
				cur = tb.next;
			}
			bin.close();
			ret.cur = ret.pos = 0;
			return ret;
		}
	};
}
#endif