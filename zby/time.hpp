#ifndef TIME_HPP
#define TIME_HPP

struct mytime {
        char date, hr, mn;

	mytime operator+ (const mytime &rhs) {
		mytime ret;
		ret.mn = mn + rhs.mn;
		ret.hr = hr + rhs.hr + ret.mn / 60; ret.mn %= 60;
		ret.date = date + rhs.date + ret.hr / 60; ret.hr %= 60;
		return ret;
	}

	mytime operator- (const mytime &rhs) {
		mytime ret;
		ret.mn = mn - rhs.mn;
		ret.hr = hr - rhs.hr;
		if (ret.mn < 0) {ret.mn += 60; ret.hr--;}
		ret.date = date - rhs.date;
		if (ret.hr < 0) {ret.hr += 60; ret.date--;}
		return ret;
	}

	/*bool operator< (const mytime &rhs) {
		int t1 = date * 3600 + hr * 60 + mn;
		int t2 = rhs.date * 3600 + rhs.hr * 60 + rhs.mn * 60;
		return t1 < t2;
	}*/

	mytime() {
		date = hr = mn = 0;
	}

	mytime (const string &t) {
		date = 0;
		if (t[0] == 'X') {
			hr = mn = 0;
			return;
		}
		hr = (t[0] - '0') * 10 + t[1] - '0';
		mn = (t[3] - '0') * 10 + t[4] - '0';
	}

	mytime &operator= (const string &t) {
		date = 0;
		if (t[0] == 'X') {
			hr = mn = 0;
			return *this;
		}
		hr = (t[0] - '0') * 10 + t[1] - '0';
		mn = (t[3] - '0') * 10 + t[4] - '0';
		return *this;
	}

	mytime &operator= (const int &x) {
		int tmp = x;
		date = tmp / 3600; tmp %= 3600;
		hr = tmp / 60; tmp %= 60;
		mn = tmp;
		return *this;
	}

	mytime &operator= (const mytime &rhs) {
		date = rhs.date;
		hr = rhs.hr;
		mn = rhs.mn;
		return *this;
	}

	operator int () const noexcept{
		int ret = date * 3600 + hr * 60 + mn;
		return ret;
	}
};

struct mydate {
        short yr;
        char mn, dy;

	mydate() {
		yr = mn = dy = 0;
	}

	mydate(const string &t) {
		dy = (t[8] - '0') * 10 + t[9] - '0';
		mn = (t[5] - '0') * 10 + t[6] - '0';
		yr = (t[0] - '0') * 1000 + (t[1] - '0') * 100 + (t[2] - '0') * 10 + t[3] - '0';
	}

	operator string() {
		string ret;
		ret += to_string(yr); ret += '-';
		if (mn < 10) ret += '0'; ret += to_string(mn); ret += '-';
		if (dy < 10) ret += '0'; ret += to_string(dy);
		return ret;
	}

	mydate &operator +=(const int &x) {
		dy++;
		if (dy == 30) {
			dy = 1; mn++;
		}
		return (*this);
	}
};

#endif
