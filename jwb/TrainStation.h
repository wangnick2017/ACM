#ifndef TRIANSTATION_HPP
#define TRAINSTATION_HPP
#include "string.h"
#include "vector.h"
class Station
{
public:
	stl::string<20> name;
	double price[5];
	stl::string<5> timeArrive, timeStart, timeStopover;
};

class Train
{
public:
	stl::string<20> trainID, name, ticketKind[5];
	int numStation, numPrice;
	stl::vector<Station> stations;
	bool IsSaled = false;
};
#endif