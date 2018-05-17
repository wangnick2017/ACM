#ifndef TRAIN_HPP
#define TRAIN_HPP

#include "string.hpp"
#include "time.hpp"

struct station {
	tstring<40> name;
	tstring<5> t_arrive, t_start, t_stop;
        //time ta, ts;
	double price[5];
};

struct Train {
	tstring<20> id;
	char catalog;
	tstring<40> name;
	tstring<40> priceName[5];
	int stationNum, priceNum;
	bool sale;
	station sta[60];
};

#endif
