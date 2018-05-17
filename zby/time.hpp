#ifndef TIME_HPP
#define TIME_HPP


#include "string.hpp"
struct time {
	int date, hr, mn;

	time operator+ (const time &rhs) {
		time ret;
		ret.mn = mn + rhs.mn;
		ret.hr = hr + rhs.hr + ret.mn / 60; ret.mn %= 60;
		ret.date = date + rhs.date + ret.hr / 60; ret.hr %= 60;
		return ret;
	}

	time operator- (const time &rhs) {
		time ret;
		ret.mn = mn - rhs.mn;
		ret.hr = hr - rhs.hr;
		if (ret.mn < 0) {ret.mn += 60; ret.hr--;}
		ret.date = date - rhs.date;
		if (ret.hr < 0) {ret.hr += 60; ret.date--;}
		return ret;
	}

	bool operator< (const time &rhs) {
		int t1 = date * 3600 + hr * 60 + mn;
		int t2 = rhs.date * 3600 + rhs.hr * 60 + rhs.mn * 60;
		return t1 < t2;
	}

	time() {
		date = hr = mn = 0;
	}

	time (const string &t) {
		date = 0;
		if (t[0] == 'X') {
			hr = mn = 0;
			return;
		}
		hr = (t[0] - '0') * 10 + t[1] - '0';
		mn = (t[3] - '0') * 10 + t[4] - '0';
	}
};

#endif
