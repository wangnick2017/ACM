#ifndef TRAIN_HPP
#define TRAIN_HPP

#include "string.hpp"
#include "time.hpp"

struct station {
        tstring<15> name;
	tstring<5> t_arrive, t_start, t_stop;
	mytime ta, ts;
        float price[5];
};

struct Train {
        tstring<12> id;
	char catalog;
        tstring<24> name;
        tstring<12> priceName[5];
        char stationNum, priceNum;
        char sale;
        station sta[46];
};

#endif
