#ifndef SHELL_HPP
#define SHELL_HPP
#include "User.h"
#include "Ticket.h"
#include "TrainStation.h"
extern User cur;
//User
const stl::string<20> &Register(const User &user)
{

}
int Login(const stl::string<20> &name, const stl::string<20> &password)
{

}
const User &QueryProfile(const stl::string<20> &ID)
{

}
int ModifyPrivlege(const stl::string<20> &ID1, const stl::string<20> &ID2, int privilege)
{

}
const stl::vector<User> &AllUsers()
{

}
//Ticket
const stl::vector<Ticket> &QueryTicket(const stl::string<20> &loc1, const stl::string<20> &loc2, const stl::string<10> &date, const stl::string<10> &catalog, bool transfer)
{

}
const stl::vector<Ticket> &QueryTransfer(const stl::string<20> &loc1, const stl::string<20> &loc2, const stl::string<10> &date, const stl::string<10> &catalog, bool transfer)
{

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
#endif