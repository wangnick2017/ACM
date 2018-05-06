#ifndef STRING_HPP
#define STRING_HPP
#include <cstring>
namespace stl
{
	template <int size>
	class string
	{
		template<int s1, int s2>
		friend string<s1 + s2> operator+(const string<s1> &a, const string<s2> &b);
		template<int s>
		friend char* operator+(const string<s> &a, const char *b);
	private:
		char data[size];
		int len = 0;
	public:
		string()
		{
			len = 0;
		}
		string(const char *s)
		{
			len = size < (int)strlen(s) ? size : (int)strlen(s);
			for (int i = 0; i < len; i++)
				data[i] = s[i];
			data[len] = '\0';
		}
		string(char *s)
		{
			len = size < (int)strlen(s) ? size : (int)strlen(s);
			for (int i = 0; i < len; i++)
				data[i] = s[i];
			data[len] = '\0';
		}
		template<int s1>
		string(string<s1> &s)
		{
			len = size < s.length() ? size : s.length();
			for (int i = 0; i < len; i++)
				data[i] = s[i];
			data[len] = '\0';
		}
		/*string(int x)
		{
			char tmp[size];
			int num = 0;
			while (x > 0) {
				tmp[num++] = x % 10 + '0';
				x /= 10;
			}
			for (int i = 0; i < num; ++i)
				data[i] = tmp[num - i - 1];
			data[num] = '\0';
		}*/
		template<int s1>
		string<size> &operator=(const string<s1> &s)
		{
			len = size < s.length() ? size : s.length();
			for (int i = 0; i < len; i++)
				data[i] = s[i];
			data[len] = '\0';
			return *this;
		}
		string<size> &operator=(char *s)
		{
			len = size < (int)strlen(s) ? size : (int)strlen(s);
			for (int i = 0; i < len; i++)
				data[i] = s[i];
			data[len] = '\0';
			return *this;
		}
		int length() const
		{
			return len;
		}
		char *stochar() const
		{
			char c[size];
			for (int i = 0; i < len; ++i)
				c[i] = data[i];
		}
		void print() const
		{
			printf("%s", data);
		}
		int stoi() const
		{
			int ret = 0;
			for (int i = len - 1; i >= 0; i--)
				ret = ret * 10 + data[i] - '0';
			return ret;
		}
		operator bool() const
		{
			return len != 0;
		}
		char operator[](int pos) const
		{
			if (pos > len)
				return '\0';
			return data[pos];
		}
		template <int s2>
		bool operator==(string<s2> &s) const
		{
			if (s.length() != len)
				return false;
			for (int i = 0; i < len; i++)
				if (data[i] != s[i])
					return false;
			return  true;
		}
		template <int s2>
		bool operator<(string<s2> &s) const
		{
			int tlen = len < s.length() ? len : s.length();
			for (int i = 0; i < tlen; i++) {
				if (data[i] < s[i])
					return true;
				if (data[i] > s[i])
					return false;
			}
			if (len < s.length())
				return true;
			return  false;
		}
	};
	template<int s1, int s2>
	string<s1 + s2> operator+(const string<s1> &a, const string<s2> &b)
	{
		string<s1 + s2> c;
		for (int i = 0; i < a.len; ++i)
			c.data[i] = a.data[i];
		for (int i = 0; i < b.len; ++i)
			c.data[i + a.len] = b.data[i];
		c.len = a.len + b.len;
		c.data[c.len] = '\0';
		return c;
	}
	template<int s>
	char* operator+(const string<s> &a, const char *b)
	{
		char *c = new char[a.len + strlen(b) + 1];
		for (int i = 0; i < a.len; ++i)
			c[i] = a.data[i];
		for (int i = 0; i < strlen(b); ++i)
			c[i + a.len] = b[i];
		c[a.len + strlen(b)] = '\0';
		return c;
	}
}
#endif