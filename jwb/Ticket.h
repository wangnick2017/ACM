#ifndef TICKET_HPP
#define TICKET_HPP
#include "string.h"
#include "vector.h"
class Ticket
{
private:
	stl::string<20> trainID, locFrom, locTo, ticketKind[5];
	stl::string<10> dateFrom, dateTo;
	stl::string<5> timeFrom, timeTo;
	int num[5];
	double price[5];
public:
	void setTrainID(const stl::string<20> &s)
	{
		trainID = s;
	}
	void setLocFrom(const stl::string<20> &s)
	{
		locFrom = s;
	}
	void setLocTo(const stl::string<20> &s)
	{
		locTo = s;
	}
	void setDataFrom(const stl::string<10> &s)
	{
		dateFrom = s;
	}
	void setDataTo(const stl::string<10> &s)
	{
		dateTo = s;
	}
	void setTimeFrom(const stl::string<5> &s)
	{
		timeFrom = s;
	}
	void setTimeTo(const stl::string<5> &s)
	{
		timeTo = s;
	}
};
#endif