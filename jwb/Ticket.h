#ifndef TICKET_HPP
#define TICKET_HPP
#include "string.h"
#include "vector.h"
class Ticket
{
public:
	stl::string<20> trainID, locFrom, locTo, ticketKind[5];
	stl::string<10> dateFrom, dateTo, catalog;
	stl::string<5> timeFrom, timeTo;
	int num[5], ticketKindNum;
	double price[5];
	Ticket(const Ticket &other)
	{
		trainID = other.trainID;
		locFrom = other.locFrom;
		locTo = other.locTo;
		ticketKindNum = other.ticketKindNum;
		for (int i = 0; i < ticketKindNum; ++i) {
			ticketKind[i] = other.ticketKind[i];
			num[i] = other.num[i];
			price[i] = other.price[i];
		}
		dateFrom = other.dateFrom;
		dateTo = other.dateTo;
		catalog = other.catalog;
		timeFrom = other.timeFrom;
		timeTo = other.timeTo;
	}
	Ticket(Ticket &&other)
	{
		trainID = other.trainID;
		locFrom = other.locFrom;
		locTo = other.locTo;
		ticketKindNum = other.ticketKindNum;
		ticketKind = other.ticketKind;
		other.ticketKind = nullptr;
		num = other.num;
		other.num = nullptr;
		price = other.price;
		other.price = nullptr;
		dateFrom = other.dateFrom;
		dateTo = other.dateTo;
		catalog = other.catalog;
		timeFrom = other.timeFrom;
		timeTo = other.timeTo;
	}
};
#endif