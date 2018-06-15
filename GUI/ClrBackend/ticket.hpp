#ifndef TICKET_HPP
#define TICKET_HPP
#include "string.hpp"

struct Ticket {
        tstring<12> trainID;
        short ticketNum;
	tstring<10> date;
        char start, end, kind;
        short ticketID;
};

struct tic {
        short a[5][46];
	tic() {
		for(int i = 0; i < 5; i++) 
                        for(int j = 0; j < 46; j++)
				a[i][j] = 2000;
	}
};

#endif
