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
};
#endif