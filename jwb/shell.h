#ifndef SHELL_HPP
#define SHELL_HPP
#include <iostream>
#include "User.h"
#include "Ticket.h"
#include "TrainStation.h"
#include "DB.h"
#include "string.h"
#include "vector.h"
class shell
{
private:
	User cur;
	int userSize = 0;
	DB<User, stl::string<20>> UserDB;
	DB<Ticket, stl::string<20>> TicketDB;
	DB<Train, stl::string<20>> TrainDB;
	DB<Station, stl::string<20>> StationDB;
public:
	void init()
	{
		
		userSize = 0;
		UserDB.init("user", 1);
		TicketDB.init("ticket", 2);
		TrainDB.init("train", 1);
		StationDB.init("station", 1);
	}
	//User
	const stl::string<20> &Register(const User &user)
	{
		int tmp = UserDB.insert(user, &(user.ID));
		if (tmp)
			return user.ID;
		return "";
	}
	int Login(const stl::string<20> &ID, const stl::string<20> &password)
	{
		User *tmp = UserDB.find_main(ID).first;
		if (tmp != NULL && tmp->password == password) {
			cur = *tmp;
			delete tmp;
			return 1;
		}
		delete tmp;
		return 0;
	}
	const User &QueryProfile(const stl::string<20> &ID)
	{
		return *(UserDB.find_main(ID).first);
	}
	int MidifuProfile(const User &user)
	{
		return UserDB.modify(user.ID, user, &(user.ID));
	}
	int ModifyPrivlege(const stl::string<20> &ID1, const stl::string<20> &ID2, int privilege)
	{
		User *tmp1 = UserDB.find_main(ID1).first;
		if (tmp1->privilege == 1) {
			delete tmp1;
			return 0;
		}
		User *tmp2 = UserDB.find_main(ID2).first;
		if (tmp2->privilege == 2 && privilege == 1) {
			delete tmp1;
			delete tmp2;
			return 0;
		}
		tmp2->privilege = privilege;
		UserDB.modify(ID2, *tmp2, &(tmp2->ID));
		delete tmp1;
		delete tmp2;
		return 1;
	}
	const stl::vector<User> &AllUsers()
	{

	}
	//Ticket
	const stl::vector<Ticket> &QueryTicket(const stl::string<20> &loc1, const stl::string<20> &loc2, const stl::string<10> &date, const stl::string<10> &catalog)
	{
		stl::pair<Ticket*, int> tmp = TicketDB.find_sub(loc1, 1);
		stl::vector<Ticket> t;
		for (int i = 0; i < tmp.second; ++i)
			if (tmp.first[i].locTo == loc2 && tmp.first[i].dateFrom == date && tmp.first[i].catalog == catalog)
				t.push_back(tmp.first[i]);
		delete tmp.first;
		return t;
	}
	const stl::vector<Ticket> &QueryTransfer(const stl::string<20> &loc1, const stl::string<20> &loc2, const stl::string<10> &date, const stl::string<10> &catalog)
	{
		stl::pair<Ticket*, int> tmp1 = TicketDB.find_sub(loc1, 1);
		stl::vector<Ticket> t;
		stl::pair<Ticket*, int> tmp2 = TicketDB.find_sub(loc2, 2);
		for(int i = 0; i < tmp1.second; ++i)
			if(tmp1.first[i].dateFrom == date && tmp1.first[i].dateTo == date && tmp1.first[i].catalog == catalog)
				for(int j = 0; j < tmp2.second; ++j)
					if (tmp2.first[j].locFrom == tmp1.first[i].dateTo && tmp2.first[j].catalog == catalog) {
						t.push_back(tmp1.first[i]);
						t.push_back(tmp2.first[j]);
					}
		delete tmp1.first;
		delete tmp2.first;
		return t;
	}
	int BuyTicket(const stl::string<20> &ID, int num, const stl::string<20> &trainID, const stl::string<20> &loc1, const stl::string<20> &loc2, const stl::string<10> &date, const stl::string<20> &ticketKind)
	{

	}
	const stl::vector<Ticket> &QueryOrder(const stl::string<20> &ID, const stl::string<10> &date, const stl::string<10> &catalog)
	{

	}
	int RefundTicket(const stl::string<20> &ID, int num, const stl::string<20> &trainID, const stl::string<20> &loc1, const stl::string<20> &loc2, const stl::string<10> &date, const stl::string<20> &ticketKind)
	{

	}
	//TrainStation
	int AddTrain(const Train &train)
	{

	}
	int SaleTrain(const stl::string<20> &trainID)
	{

	}
	const Train &QueryTrain(const stl::string<20> &trainID)
	{

	}
	int DeleteTrain(const stl::string<20> &trainID)
	{

	}
	int ModifyTrain(const Train &train)
	{

	}
	const stl::vector<Train> &AllTrains()
	{

	}
};
#endif