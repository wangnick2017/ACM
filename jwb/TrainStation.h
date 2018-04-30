#ifndef TRIANSTATION_HPP
#define TRAINSTATION_HPP
#include "string.h"
#include "vector.h"
class Station
{
private:
	stl::string<20> name;
	double price[5];
	stl::string<5> timeArrive, timeStart, timeStopover;
public:
	void setName(const stl::string<20> &s)
	{
		name = s;
	}
	void setTimeArrive(const stl::string<5> &s)
	{
		timeArrive = s;
	}
	void setTimeStart(const stl::string<5> &s)
	{
		timeStart = s;
	}
	void setTimeStopover(const stl::string<5> &s)
	{
		timeStopover = s;
	}
};

class Train
{
private:
	stl::string<20> trainID, name, ticketKind[5];
	int numStation, numPrice;
	stl::vector<Station> stations;
	bool IsSaled = false;
public:
	void setTrainID(const stl::string<20> &s)
	{
		trainID = s;
	}
	void setName(const stl::string<20> &s)
	{
		name = s;
	}
	void setNumStation(const int &x)
	{
		numStation = x;
	}
	void setNumPrice(const int &x)
	{
		numPrice = x;
	}
	void setIsSaled(const bool &x)
	{
		IsSaled = x;
	}
};
#endif