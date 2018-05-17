#ifndef TICKET_HPP
#define TICKET_HPP
#include "string.hpp"

struct Ticket {
	tstring<20> trainID, catalog;
	int left[5], ticketNum;
	double price[5];
	tstring<40> ticketKind[5];
	tstring<40> locFrom, locTo;
	tstring<10> dateFrom, dateTo;
	tstring<5> timeFrom, timeTo;
};

struct tic {
	int a[60];
	tic() {
		for(int i = 0; i < 60; i++) a[i] = 2000;
	}
};

#endif