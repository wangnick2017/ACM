#ifndef TICKET_HPP
#define TICKET_HPP
#include "string.hpp"

struct Ticket {
	tstring<20> trainID;
	int ticketNum;
	tstring<10> date;
	int start, end, kind;
	int ticketID;
};

struct tic {
	int a[5][60];
	tic() {
		for(int i = 0; i < 5; i++) 
			for(int j = 0; j < 60; j++) 
				a[i][j] = 2000;
	}
};

#endif