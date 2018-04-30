#ifndef STRING_HPP
#define STRING_HPP
#include <cstring>
namespace stl
{
	template <int size>
	class string
	{
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
		}
		string(char *s)
		{
			len = size < (int)strlen(s) ? size : (int)strlen(s);
			for (int i = 0; i < len; i++)
				data[i] = s[i];
		}
		template<int s1>
		string(string<s1> &s)
		{
			len = size < s.length() ? size : s.length();
			for (int i = 0; i < len; i++)
				data[i] = s[i];
		}
		template<int s1>
		string<size> operator=(const string<s1> &s)
		{
			len = size < s.length() ? size : s.length();
			for (int i = 0; i < len; i++)
				data[i] = s[i];
			return *this;
		}
		string<size> operator=(char *s)
		{
			len = size < (int)strlen(s) ? size : (int)strlen(s);
			for (int i = 0; i < len; i++)
				data[i] = s[i];
			return *this;
		}
		int length()
		{
			return len;
		}
		int stoi()
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
		char operator[](int pos)
		{
			if (pos > len)
				return '\0';
			return data[pos];
		}
		template <int s2>
		bool operator==(string<s2> &s)
		{
			if (s.length() != len)
				return false;
			for (int i = 0; i < len; i++)
				if (data[i] != s[i])
					return false;
			return  true;
		}
		template <int s2>
		bool operator<(string<s2> &s)
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
}
#endif