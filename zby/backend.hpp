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
#include <cstring>
#include <string>

//using namespace sjtu;

class Backend {
private:
	DB<int, User> userDB;
	DB<tstring<20>, Train> trainDB;
	DB<tstring<100>, char> routeDB;
	DB<tstring<100>, char> ticketDB;
	DB<tstring<30>, tic> train_ticDB;

public:
	char errorMessage[20];

	void init() {
		userDB.init("user");
		trainDB.init("train");
		ticketDB.init("ticket");
		routeDB.init("route");
		train_ticDB.init("train_tic");
	}

	int regist(char *msg = NULL) {
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

	int login(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		int id = buffer.geti();
		tstring<20> pwd;
		pwd = buffer.gets();
		auto ret = userDB.find(id);
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

	char *query_profile(int id) {
		auto ret = userDB.find(id);
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

	int modify_privilege(int id1, int id2, int pri) {
		auto ret1 = userDB.find(id1);
		auto ret2 = userDB.find(id2);
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
		userDB.modify(ret2.first->id, *(ret2.first));
		delete ret1.first;
		delete ret2.first;
		return 1;
	}

	int add_train(char *msg = NULL) {
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
		for(int i = 0; i < train.stationNum; i++) {
			train.sta[i].name = buffer.gets();
			train.sta[i].t_arrive = buffer.gets();
			train.sta[i].t_start = buffer.gets();
			train.sta[i].t_stop = buffer.gets();
			for(int j = 0; j < train.priceNum; j++) {
				string tmp = buffer.gets();
				while (tmp[0] < '0' || tmp[0] > '9') tmp.erase(0, 1);
				train.sta[i].price[j] = stod(tmp);
			}
		}
		train.sale = false;
		bool ret = trainDB.insert(train.id, train);
		return ret;
	}

	int sale_train(char *msg = NULL) {
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
			for (int j = i + 1; j < ret.first->stationNum; j++) {
				tstring<100> route;
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

	char *query_train(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		tstring<20> tid;
		tid = buffer.gets();
		auto ret = trainDB.find(tid);
		if (!ret.second) {
			char *res = new char[2];
			res[0] = '0';
			res[1] = '\0';
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
				str += '$'; str += to_string(ret.first->sta[i].price[j]); str += ' ';
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

	int delete_train(char *msg = NULL) {
		scanner buffer;
		char *tmsg = msg;
		buffer.set(tmsg);
		tstring<20> tid;
		tid = buffer.gets();
		auto ret = trainDB.find(tid);
		if (!ret.second) {
			return 0;
		}
		trainDB.erase(ret.first->id);
		delete ret.first;
		return 1;
	}

	int modify_train(char *msg = NULL) {
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

	char *query_ticket(char *msg = NULL) {
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
		sjtu::vector<tstring<100>> tvec;
		tstring<100> route1, route2;
		route1 += (string)loc1; route1 += (string)loc2;
		route2 += (string)loc1; route2 += (string)loc2;
		tstring<20> tid;
		tid.len = 20;
		for(int i = 0; i < 20; i++) tid.m[i] = 0; 
		route1 += (string) tid;
		for(int i = 0; i < 20; i++) tid.m[i] = 255;
		route2 += (string) tid;
		routeDB.findRange(route1, route2, tvec);
		tstring<80> troute;
		troute += (string)loc1; troute += (string)loc2;
		int tl = troute.len;
		int tot = 0;
		string ans;
		for (int i = 0; i < tvec.size(); i++) {
			tstring<20> rid;
			rid.len = tvec[i].len - tl;
			for(int j = tl; j < tvec[i].len; j++) rid.m[j - tl] = tvec[i].m[j];
			auto res = trainDB.find(rid);
			if(!res.second) continue;
			bool flag = false;
			for(int j = 0; j < cata.len; j++) {
				if (cata[i] == res.first->catalog) {
					flag = true;
					break;
				}
			}
			if (!flag) continue;
			ans += (string)rid; ans += ' ';
			ans += (string)loc1; ans += ' ';
			ans += (string)date; ans += ' ';
			int tl, tr;
			for (int j = 0; j < res.first->stationNum; j++) {
				if (res.first->sta[j].name == loc1) {
					ans += (string)res.first->sta[j].t_start; ans += ' ';
					ans += (string)loc2; ans += ' ';
					ans += (string)date; ans += ' ';
					tl = j;
				}
				if (res.first->sta[j].name == loc2) {
					ans += (string)res.first->sta[j].t_arrive; ans += ' ';
					tr = j;
					break;
				}
			}
			ans += ' ';
			for(int j = 0; j < res.first->priceNum; j++) {
				ans += (string)res.first->priceName[j]; ans += ' ';
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
						if (res2.first->a[k] < rest_tic) rest_tic = res2.first->a[k];
					}
				}
				ans += to_string(rest_tic); ans += ' ';
				ans += to_string(res.first->sta[tr].price[j] - res.first->sta[tl].price[j]); ans += ' ';
			}
			ans += '\n';
			delete res.first;
		}
		char *ret = new char[ans.length() + 1];
		int len = ans.length();
		for(int i = 0; i < len; i++) ret[i] = ans[i];
		ret[len] = '\0';
		return ret;
	}

};

#endif 

