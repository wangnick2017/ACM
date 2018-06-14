#ifndef BACKEND_HPP
#define BACKEND_HPP
#include "user.hpp"
#include "string.hpp"
#include "ticket.hpp"
#include "train.hpp"
#include "vector.hpp"
#include "tokenscanner.hpp"
#include "DB.hpp"
#include "time.hpp"
#include "fvector.h"
#include <cstring>
#include <string>

using std::string;

using std::to_string;

fvector<int, User> userDB;
DB<tstring<20>, Train, cmp> trainDB;
DB<tstring<60>, char, cmp> routeDB;
DB<tstring<40>, Ticket, cmp> ticketDB;
DB<tstring<30>, tic, cmp> train_ticDB;
DB<tstring<20>, char, cmp> staDB;

class Backend {

    

public:
	static char errorMessage[20];

	static void init() {
		userDB.init("user");
		trainDB.init("train");
		ticketDB.init("ticket");
		routeDB.init("route");
		train_ticDB.init("train_tic");
		staDB.init("sta");
		tstring<40> foo;
		auto bar = ticketDB.find(foo);
		if (!bar.second) {
			Ticket tmp;
			tmp.ticketID = 0;
			ticketDB.insert(foo, tmp);
		}
        else{
            delete bar.first;
        }
	}

    static void clear() {
		userDB.trunc();
		trainDB.trunc();
		ticketDB.trunc();
		routeDB.trunc();
		train_ticDB.trunc();
		staDB.trunc();
	}

    static int regist(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		User user;
		user.name = buffer.gets();
		user.password = buffer.gets();
		user.email = buffer.gets();
		user.phone = buffer.gets();
		user.id = 2018 + userDB.size();
		if (user.id == 2018) user.privilege = 2;
		else user.privilege = 1;
		userDB.insert(user.id, user);
		return user.id;
	}

    static int login(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		int id = buffer.geti();
		tstring<20> pwd;
		pwd = buffer.gets();
        auto ret = userDB.find(id - 2018);
		if (!ret.second) {
			return 0;
		}
		if (!(ret.first->password == pwd)) {
			delete ret.first;
			return 0;
		}
		else {
			delete ret.first;
			return 1;	
		}
	}

    static char *query_profile(int id) {
        auto ret = userDB.find(id - 2018);
		if (!ret.second) {
			char *st = new char[2];
			st[0] = '0';
			st[1] = '\0';
			return st;
		}
		string str;
		str += ((string)ret.first->name);
		str += ' ';
		str += ((string)ret.first->email);
		str += ' ';
		str += ((string)ret.first->phone);
		str += ' ';
		str += to_string(ret.first->privilege);
		char *res = new char[str.length() + 1];
		int len = str.length();
		for(int i = 0; i < len; i++) res[i] = str[i];
		res[len] = '\0';
		delete ret.first;
		return res;
	}

    static int modify_profile(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		int id = buffer.geti();
                auto res = userDB.find(id - 2018);
		if (!res.second) return 0;
		User user;
		user.name = buffer.gets();
		user.password = buffer.gets();
		user.email = buffer.gets();
		user.phone = buffer.gets();
		user.id = id;
		user.privilege = res.first->privilege;
        userDB.modify(id - 2018, user);
		delete res.first;
		return 1;
	}

    static int modify_privilege(int id1, int id2, int pri) {
        auto ret1 = userDB.find(id1 - 2018);
        auto ret2 = userDB.find(id2 - 2018);
		if (!ret1.second || !ret2.second) {
			if (ret1.second) delete ret1.first;
			if (ret2.second) delete ret2.first;
			return 0;
		}
		if (ret1.first->privilege != 2) {
			delete ret1.first;
			delete ret2.first;
			return 0;
		}
		if (ret2.first->privilege > pri) {
			delete ret1.first;
			delete ret2.first;
			return 0;
		}
		ret2.first->privilege = pri;
                userDB.modify(ret2.first->id - 2018, *(ret2.first));
		delete ret1.first;
		delete ret2.first;
		return 1;
	}

    static int add_train(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		Train train;
		train.id = buffer.gets();
		train.name = buffer.gets();
		train.catalog = buffer.getc();
		train.stationNum = buffer.geti();
		train.priceNum = buffer.geti();
		for(int i = 0; i < train.priceNum; i++) train.priceName[i] = buffer.gets();
		mytime start, last;
		double totp = 0;
		for(int i = 0; i < train.stationNum; i++) {
			train.sta[i].name = buffer.gets();
			train.sta[i].t_arrive = buffer.gets();
			train.sta[i].t_start = buffer.gets();
			train.sta[i].t_stop = buffer.gets();
			if (i == 0) {
				start = (string)train.sta[i].t_start;
				last = start;
				train.sta[i].ta = start;
				train.sta[i].ts = start;
			}
			else {
				train.sta[i].ta = (string)train.sta[i].t_arrive;
				train.sta[i].ta.date = last.date;
				if (train.sta[i].ta < last) train.sta[i].ta.date++;
				train.sta[i].ts = (string)train.sta[i].t_start;
				train.sta[i].ts.date = last.date;
				if (train.sta[i].ts < last) train.sta[i].ts.date++;
				last = train.sta[i].ts;
			}
			for(int j = 0; j < train.priceNum; j++) {
				string tmp = buffer.gets();
				while (tmp[0] < '0' || tmp[0] > '9') tmp.erase(0, 1);
				train.sta[i].price[j] = stod(tmp);// + totp;
				//totp = train.sta[i].price[j];
			}
		}
		train.sale = false;
		bool ret = trainDB.insert(train.id, train);
		return ret;
	}

    static int sale_train(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		tstring<20> tid;
		tid = buffer.gets();
		auto ret = trainDB.find(tid);
		if (!ret.second) return 0;
		if (ret.first->sale) {
			delete ret.first;
			return 0;
		}
		ret.first->sale = 1;
		for(int i = 0; i < ret.first->stationNum; i++) {
			staDB.insert(ret.first->sta[i].name, 1);
			for (int j = i + 1; j < ret.first->stationNum; j++) {
				tstring<60> route;
				route += (string)ret.first->sta[i].name;
				route += (string)ret.first->sta[j].name;
				route += (string)ret.first->id;
				routeDB.insert(route, 1);
			}
		}
		trainDB.modify(ret.first->id, *(ret.first));
		delete ret.first;
		return 1;
	}

    static char *query_train(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		tstring<20> tid;
		tid = buffer.gets();
		auto ret = trainDB.find(tid);
		if (!ret.second) {
			char *res = new char[3];
			res[0] = '0';
			res[2] = '\0'; res[1] = '\n';
			return res;
		}
		string str;
		str += (string)ret.first->id; str += ' ';
		str += (string)ret.first->name; str += ' ';
		str += ret.first->catalog; str += ' ';
		str += to_string(ret.first->stationNum); str += ' ';
		str += to_string(ret.first->priceNum); str += ' ';
		for(int i = 0; i < ret.first->priceNum; i++) {
			str += (string)ret.first->priceName[i];
			str += ' ';
		}
		for (int i = 0; i < ret.first->stationNum; i++) {
			str += '\n';
			str += (string)ret.first->sta[i].name; str += ' ';
			str += (string)ret.first->sta[i].t_arrive; str += ' ';
			str += (string)ret.first->sta[i].t_start; str += ' ';
			str += (string)ret.first->sta[i].t_stop; str += ' ';
			for (int j = 0; j < ret.first->priceNum; j++) {
				str += to_string(ret.first->sta[i].price[j]); str += ' ';
			}
		}
		str += '\n';
		char *res = new char[str.length() + 1];
		int len = str.length();
		for(int i = 0; i < len; i++) res[i] = str[i];
		res[len] = '\0';
		delete ret.first;
		return res;
	}

    static int delete_train(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		tstring<20> tid;
		tid = buffer.gets();
		auto ret = trainDB.find(tid);
		if (!ret.second) {
			return 0;
		}
        if (ret.first->sale) {
            delete ret.first;
            return 0;
        }
		trainDB.erase(ret.first->id);
		delete ret.first;
		return 1;
	}

    static int modify_train(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		tstring<20> tid;
		tid = buffer.gets();
		auto ret = trainDB.find(tid);
		if (!ret.second) {
			return 0;
		}
		if (ret.first->sale) {
			delete ret.first;
			return 0;
		}
		ret.first->name = buffer.gets();
		ret.first->catalog = buffer.getc();
		ret.first->stationNum = buffer.geti();
		ret.first->priceNum = buffer.geti();
		for(int i = 0; i < ret.first->priceNum; i++) ret.first->priceName[i] = buffer.gets();
		for(int i = 0; i < ret.first->stationNum; i++) {
			ret.first->sta[i].name = buffer.gets();
			ret.first->sta[i].t_arrive = buffer.gets();
			ret.first->sta[i].t_start = buffer.gets();
			ret.first->sta[i].t_stop = buffer.gets();
			for(int j = 0; j < ret.first->priceNum; j++) {
				string tmp = buffer.gets();
				while (tmp[0] < '0' || tmp[0] > '9') tmp.erase(0, 1);
				ret.first->sta[i].price[j] = stod(tmp);
			}
		}
		bool res = trainDB.modify(ret.first->id, *(ret.first));
		delete ret.first;
		return res;
	}

    static string add_date(string ta, int tb) {
		string ret = ta;
		if (tb) {
			if (ret[8] == '3' && ret[9] == '0') {
                                ret[8] = '0'; ret[9] = '1';
				ret[6]++;
				return ret;
			}
			if (ret[9] != '9') {
				ret[9]++;
				return ret;
			}
			ret[8]++; ret[9] = '0'; return ret;
		}
		return ret;
	}

    static char *query_ticket(char *msg = NULL) {
		char *tmsg = msg;
		scanner buffer;
		buffer.set(tmsg);
		tstring<20> loc1;
		loc1 = buffer.gets();
		tstring<20> loc2;
		loc2 = buffer.gets();
		tstring<10> date;
		date = buffer.gets();
		tstring<10> cata;
		cata = buffer.gets();
		sjtu::vector<tstring<60>> tvec;
		tstring<60> route1, route2;
		route1 += (string)loc1; route1 += (string)loc2;
		route2 += (string)loc1; route2 += (string)loc2;
		tstring<20> tid;
		tid.len = 20;
		for(int i = 0; i < 20; i++) tid.m[i] = -128; 
		route1 += (string) tid;
		for(int i = 0; i < 20; i++) tid.m[i] = 127;
		route2 += (string) tid;
		routeDB.findRange(route1, route2, tvec);
                //printf("%d\n", tvec.size());
		tstring<80> troute;
		troute += (string)loc1; troute += (string)loc2;
		int tl = troute.len;
		int tot = 0;
		string ans;
		for (int i = 0; i < tvec.size(); i++) {
			tstring<20> rid;
			string tmp;
            rid.len = tvec[i].len - tl;
			for(int j = tl; j < tvec[i].len; j++) rid.m[j - tl] = tvec[i].m[j];
			auto res = trainDB.find(rid);
            if (!res.second) continue;
			bool flag = false;
			for(int j = 0; j < cata.len; j++) {
				if (cata[j] == res.first->catalog) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				delete res.first;
				continue;
			}
			tmp += (string)rid; tmp += ' ';
			tmp += (string)loc1; tmp += ' ';
			int tl, tr;
			for (int j = 0; j < res.first->stationNum; j++) {
				if (res.first->sta[j].name == loc1) {
                    tmp += (string)add_date(date, res.first->sta[j].ts.date); tmp += ' ';
					tmp += (string)res.first->sta[j].t_start; tmp += ' ';
					tmp += (string)loc2; tmp += ' ';
					tl = j;
				}
				if (res.first->sta[j].name == loc2) {
                    tmp += (string)add_date(date, res.first->sta[j].ts.date); tmp += ' ';
					tmp += (string)res.first->sta[j].t_arrive; tmp += ' ';
					tr = j;
					break;
				}
            }
			int minr = 2000;
			for(int j = 0; j < res.first->priceNum; j++) {
				tmp += (string)res.first->priceName[j]; tmp += ' ';
				tstring<30> tk;
				tk += res.first->id; tk += date;
				auto res2 = train_ticDB.find(tk);
				int rest_tic = 2000;
				if (!res2.second) {
					tic tmp;
					train_ticDB.insert(tk, tmp);
				}
				else {
					for(int k = tl + 1; k <= tr; k++) {
						if (res2.first->a[j][k] < rest_tic) rest_tic = res2.first->a[j][k];
					}
				}
                                delete res2.first;
				tmp += to_string(rest_tic); tmp += ' ';
				double tp = 0;
                for(int k = tl + 1; k <= tr; k++) {
					tp += res.first->sta[k].price[j];
				}
				tmp += to_string(tp); tmp += ' ';
				if (rest_tic < minr) minr = rest_tic;
			}
			tmp += '\n';
			if (1/*minr ??? TODO*/) {
				ans += tmp;
				tot ++;
			}
			delete res.first;
		}
		if (tot == 0) {
			char *ret = new char[3];
			ret[0] = '0';
			ret[1] = '\n'; ret[2] = '\0';
			return ret;
		}
		ans = to_string(tot) + '\n' + ans;
		char *ret = new char[ans.length() + 1];
		int len = ans.length();
		for(int i = 0; i < len; i++) ret[i] = ans[i];
		ret[len] = '\0';
		return ret;
	}

    static mypair<mytime, tstring<20>> qtt(tstring<20> &loc1, tstring<20> &loc2, tstring<10> &date, tstring<10> &cata, mytime &mt) {
		sjtu::vector<tstring<60>> tvec;
		tstring<60> route1, route2;
		route1 += (string)loc1; route1 += (string)loc2;
		route2 += (string)loc1; route2 += (string)loc2;
		tstring<20> tid;
		tid.len = 20;
		for(int i = 0; i < 20; i++) tid.m[i] = -128; 
		route1 += (string) tid;
		for(int i = 0; i < 20; i++) tid.m[i] = 127;
		route2 += (string) tid;
		routeDB.findRange(route1, route2, tvec);
		tstring<80> troute;
		troute += (string)loc1; troute += (string)loc2;
		int tl = troute.len;
		int tot = 0;
		//string ans;
		tstring<20> ansid;
		mytime ans; ans.date = 100;
		for (int i = 0; i < tvec.size(); i++) {
			tstring<20> rid;
			rid.len = tvec[i].len - tl;
			for(int j = tl; j < tvec[i].len; j++) rid.m[j - tl] = tvec[i].m[j];
			auto res = trainDB.find(rid);
			if(!res.second) continue;
			bool flag = false;
			for(int j = 0; j < cata.len; j++) {
				if (cata[j] == res.first->catalog) {
					flag = true;
					break;
				}
			}
			if (!flag) {
				delete res.first;
				continue;
			}
			int tl, tr;
			mytime tt; tt = mt;
			for (int j = 0; j < res.first->stationNum; j++) {
				if (res.first->sta[j].name == loc1) {
                        if (res.first->sta[j].ts < mt) {
						tt.date++;
					}
				}
				if (res.first->sta[j].name == loc2) {
					tt.hr = res.first->sta[j].ta.hr;
					tt.mn = res.first->sta[j].ta.mn;
					tt.date += res.first->sta[j].ta.date;
				}
			}
            if (tt < ans) {
				ans = mt;
				ansid = rid;
			}
			delete res.first;
		}
		mypair<mytime, tstring<20>> ret(ans, ansid);
		return ret;
	}

    static char *query_transfer(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		tstring<20> loc1;
		loc1 = buffer.gets();
		tstring<20> loc2;
		loc2 = buffer.gets();
		tstring<10> date;
		date = buffer.gets();
		tstring<10> cata;
		cata = buffer.gets();
		mytime ans; ans.date = 100;
		tstring<20> id1, id2, tloc;
		tstring<20> lloc, rloc;
		for(int i = 0; i < 20; i++) {
			lloc.m[i] = -128; rloc.m[i] = 127;
		}
		lloc.len = rloc.len = 20;
		sjtu::vector<tstring<20>> tvec;
		staDB.findRange(lloc, rloc, tvec);
		for (int i = 0; i < tvec.size(); i++) {
			mytime tmp;
			auto res1 = qtt(loc1, tvec[i], date, cata, tmp);
			auto res2 = qtt(tvec[i], loc2, date, cata, res1.first);
			if (res2.first < ans) {
				ans = res2.first;
				id1 = res1.second;
				id2 = res2.second;
				tloc = tvec[i];
			}
		}
		if (ans.date == 100) {
			char *ret = new char[4];
			ret[0] = '-'; ret[1] = '1'; ret[2] = '\n'; ret[3] = '\0';
			return ret;
		}

		string tmp;
		auto res = trainDB.find(id1);
		tmp += (string)id1; tmp += ' ';
		tmp += (string)loc1; tmp += ' ';
		int tl, tr;
		mytime foo;
		for (int j = 0; j < res.first->stationNum; j++) {
			if (res.first->sta[j].name == loc1) {
                tmp += (string)add_date(date, res.first->sta[j].ts.date); tmp += ' ';
				tmp += (string)res.first->sta[j].t_start; tmp += ' ';
				tmp += (string)tloc; tmp += ' ';
				tl = j;
			}
			if (res.first->sta[j].name == tloc) {
                tmp += (string)add_date(date, res.first->sta[j].ts.date); tmp += ' ';
				tmp += (string)res.first->sta[j].t_arrive; tmp += ' ';
				foo = res.first->sta[j].ta;
				tr = j;
				break;
			}
		}
		tmp += ' ';
		int minr = 2000;
		for(int j = 0; j < res.first->priceNum; j++) {
			tmp += (string)res.first->priceName[j]; tmp += ' ';
			tstring<30> tk;
			tk += res.first->id; tk += date;
			auto res2 = train_ticDB.find(tk);
			int rest_tic = 2000;
			if (!res2.second) {
				tic tmp;
				train_ticDB.insert(tk, tmp);
			}
			else {
				for(int k = tl + 1; k <= tr; k++) {
					if (res2.first->a[j][k] < rest_tic) rest_tic = res2.first->a[j][k];
				}
			}
			tmp += to_string(rest_tic); tmp += ' ';
			double tp = 0;
                        for(int k = tl + 1; k <= tr; k++) {
				tp += res.first->sta[k].price[j];
			}
			tmp += to_string(tp); tmp += ' ';
			if (rest_tic < minr) minr = rest_tic;
		}
		tmp += '\n';

		res = trainDB.find(id2);
		tmp += (string)id2; tmp += ' ';
		tmp += (string)tloc; tmp += ' ';
		for (int j = 0; j < res.first->stationNum; j++) {
			if (res.first->sta[j].name == tloc) {
				int date_add = 0;
				if (res.first->sta[j].ts < foo) date_add = 1;
                                tmp += (string)add_date(date, res.first->sta[j].ts.date); tmp += ' ';
				tmp += (string)res.first->sta[j].t_start; tmp += ' ';
				tmp += (string)loc2; tmp += ' ';
				tl = j;
			}
			if (res.first->sta[j].name == loc2) {
				int date_add = 0;
				if (res.first->sta[j].ts < foo) date_add = 1;
                                tmp += (string)add_date(date, res.first->sta[j].ts.date); tmp += ' ';
				tmp += (string)res.first->sta[j].t_arrive; tmp += ' ';
				tr = j;
				break;
			}
		}
		tmp += ' ';
		for(int j = 0; j < res.first->priceNum; j++) {
			tmp += (string)res.first->priceName[j]; tmp += ' ';
			tstring<30> tk;
			tk += res.first->id; tk += date;
			auto res2 = train_ticDB.find(tk);
			int rest_tic = 2000;
			if (!res2.second) {
				tic tmp;
				train_ticDB.insert(tk, tmp);
			}
			else {
				for(int k = tl + 1; k <= tr; k++) {
					if (res2.first->a[j][k] < rest_tic) rest_tic = res2.first->a[j][k];
				}
			}
			tmp += to_string(rest_tic); tmp += ' ';
			double tp = 0;
                        for(int k = tl + 1; k <= tr; k++) {
				tp += res.first->sta[k].price[j];
			}
			tmp += to_string(tp); tmp += ' ';
			if (rest_tic < minr) minr = rest_tic;
		}
		tmp += '\n';
		char *ret = new char[tmp.length() + 1];
		int len = tmp.length();
		for(int i = 0; i < len; i++) ret[i] = tmp[i];
		ret[len] = '\0';
		delete res.first;
		return ret;
	}

    static int buy_ticket(char *msg = NULL) {
		char *tmsg = msg;
		scanner buffer;
		buffer.set(tmsg);
		tstring<20> id;
		id = buffer.gets();
		int num = buffer.geti();
		tstring<20> train_id;
		train_id = buffer.gets();
		tstring<20> loc1;
		loc1 = buffer.gets();
		tstring<20> loc2;
		loc2 = buffer.gets();
		tstring<10> date;
		date = buffer.gets();
		tstring<20> kind;
		kind = buffer.gets();
		auto res = trainDB.find(train_id);
		if (!res.second) return 0;
		int tl = -1, tr = -1, tk = -1;
		for (int i = 0; i < res.first->stationNum; i++) {
			if (res.first->sta[i].name == loc1) tl = i;
			if (res.first->sta[i].name == loc2) tr = i;				
		}
		if (tl < 0 || tr < 0 || tl >= tr) {
			delete res.first;
			return 0;
		}
		for (int i = 0; i < res.first->priceNum; i++) {
			if (res.first->priceName[i] == kind) tk = i;
		}
		if (tk < 0) {
			delete res.first;
			return 0;
		}
		tstring<30> tc;
		tc += res.first->id;
		tc += date;
		auto res2 = train_ticDB.find(tc);
		if (!res2.second) {
			tic tmp;
			train_ticDB.insert(tc, tmp);
			res2 = train_ticDB.find(tc);
		}
		int tic_left = 2000;
		for (int i = tl + 1; i <= tr; i++) {
			if (res2.first->a[tk][i] < tic_left) tic_left = res2.first->a[tk][i];
		}
		if (tic_left < num) {
			delete res2.first;
			delete res.first;
			return 0;
		}
		for (int i = tl + 1; i <= tr; i++) 
			res2.first->a[tk][i] -= num;
		train_ticDB.modify(tc, *(res2.first));
		tstring<40> tic_n, foo;
		tic_n += id; tic_n += date; tic_n += res.first->catalog;
		auto bar = ticketDB.find(foo);
		int tn = bar.first->ticketID + 1;
		bar.first->ticketID++;
		ticketDB.modify(foo, *(bar.first));
		tic_n += fill_to(to_string(tn), 9);
		Ticket tic;
		tic.trainID = res.first->id;
		tic.ticketNum = num;
		tic.date = date;
		tic.start = tl; tic.end = tr; tic.kind = tk;
		tic.ticketID = tn;
		ticketDB.insert(tic_n, tic);
		delete res2.first;
		delete res.first;
		delete bar.first;
		return 1;
	}

    static char *query_order(char *msg = NULL) {
		char *tmsg = msg;
		scanner buffer;
		buffer.set(tmsg);
		tstring<20> uid;
		uid = buffer.gets();
		tstring<10> date;
		date = buffer.gets();
		string cata = buffer.gets();
		string ans;
		int tot = 0;
		for (int l = 0; l < cata.length(); l++){
			tstring<40> tic1, tic2;
			tic1 += uid; tic2 += uid;
			tic1 += date; tic2 += date;
			tic1 += cata[l]; tic2 += cata[l];
			tic1 += fill_to(to_string(0), 9); tic2 += fill_to(to_string(1e9-1), 9);
			sjtu::vector<Ticket> tvec;
			ticketDB.findRangeData(tic1, tic2, tvec);
			tot += tvec.size();
			int sz = tvec.size();
			for(int i = 0; i < sz; i++) {
				auto res2 = trainDB.find(tvec[i].trainID);
				ans += (string)tvec[i].trainID; ans += ' ';
				ans += (string)res2.first->sta[tvec[i].start].name; ans += ' ';
				ans += (string)tvec[i].date; ans += ' '; ans += (string)res2.first->sta[tvec[i].start].t_start; ans += ' ';
				ans += (string)res2.first->sta[tvec[i].end].name; ans += ' ';
				ans += (string)tvec[i].date; ans += ' '; ans += (string)res2.first->sta[tvec[i].end].t_arrive; ans += ' ';
				for(int j = 0; j < res2.first->priceNum; j++) {
					ans += (string)res2.first->priceName[j]; ans += ' ';
					if (j == tvec[i].kind) ans += to_string(tvec[i].ticketNum); else ans += '0';
					ans += ' ';
					double tp = 0;
	                for(int k = tvec[i].start + 1; k <= tvec[i].end; k++) {
						tp += res2.first->sta[k].price[j];
					}
					ans += to_string(tp); ans += ' ';
				}
				ans += '\n';
				delete res2.first;	
			}
		}
		ans = to_string(tot) + '\n' + ans;
		char *ret = new char[ans.length() + 1];
		int len = ans.length();
		for(int i = 0; i < len; i++) ret[i] = ans[i];
		ret[len] = '\0';
		return ret;
	}

    static int refund_ticket(char *msg = NULL) {
		char *tmsg = msg;
		scanner buffer;
		buffer.set(tmsg);
		tstring<20> uid;
		uid = buffer.gets();
		int num = buffer.geti();
		tstring<20> tid;
		tid = buffer.gets();
		tstring<20> loc1;
		loc1 = buffer.gets();
		tstring<20> loc2;
		loc2 = buffer.gets();
		tstring<10> date;
		date = buffer.gets();
		tstring<20> kind;
		kind = buffer.gets();
		auto res2 = trainDB.find(tid);
		if (!res2.second) return 0;
		char cata = res2.first->catalog;
		tstring<40> tic1, tic2;
		tic1 += uid; tic2 += uid;
		tic1 += date; tic2 += date;
		tic1 += cata; tic2 += cata;
		tic1 += fill_to(to_string(0), 9); tic2 += fill_to(to_string((int)1e9-1), 9);
		sjtu::vector<Ticket> tvec;
		ticketDB.findRangeData(tic1, tic2, tvec);
		int tot = 0;
		for (int i = 0; i < tvec.size(); i++) if (tid == tvec[i].trainID) {
			if (res2.first->sta[tvec[i].start].name != loc1 || res2.first->sta[tvec[i].end].name != loc2 || res2.first->priceName[tvec[i].kind] != kind) continue;
			tot += tvec[i].ticketNum;
		}
		tic1 = (string)""; tic1 += uid; tic1 += date; tic1 += cata;
		if (tot < num) return 0;
		for (int i = 0; i < tvec.size(); i++) if (tid == tvec[i].trainID) {
			if (res2.first->sta[tvec[i].start].name != loc1 || res2.first->sta[tvec[i].end].name != loc2 || res2.first->priceName[tvec[i].kind] != kind) continue;
			if (num < tvec[i].ticketNum) {
				tstring<40> t_tic;
				t_tic = tic1; t_tic += fill_to(to_string(tvec[i].ticketID), 9);
				tvec[i].ticketNum -= num;
				ticketDB.modify(t_tic, tvec[i]);
				tstring<30> tc;
				tc += res2.first->id;
				tc += date;
				auto res3 = train_ticDB.find(tc);
				for (int j = tvec[i].start + 1; j <= tvec[i].end; j++) 
					res3.first->a[tvec[i].kind][j] += num;
				train_ticDB.modify(tc, *(res3.first));
				num = 0;
				delete res3.first;
			}
			else {
				tstring<40> t_tic;
				t_tic = tic1; t_tic += fill_to(to_string(tvec[i].ticketID), 9);
				num -= tvec[i].ticketNum;
				ticketDB.erase(t_tic);
				tstring<30> tc;
				tc += res2.first->id;
				tc += date;
				auto res3 = train_ticDB.find(tc);
				for (int j = tvec[i].start + 1; j <= tvec[i].end; j++) 
					res3.first->a[tvec[i].kind][j] += tvec[i].ticketNum;
				train_ticDB.modify(tc, *(res3.first));
				delete res3.first;
			}
			if (num == 0) break;
		}
		delete res2.first;
		return 1;
	}

};

#endif 

