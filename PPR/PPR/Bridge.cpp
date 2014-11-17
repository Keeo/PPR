#include "stdafx.h"
#include "Bridge.h"


void Bridge::initData(char* data, int dataLength, bool start)
{
	std::istringstream graf(data);
	game_.getGraph().load(&graf);

	if (start) {
		game_.addNextMoves(0);
	}
}

void Bridge::setWork(char* data, int dataLength)
{
	int* temp = (int*)data;
	std::vector<int> moves(temp, temp + dataLength);
	
	std::stringstream ss;
	for (auto &a : moves) {
		ss << a << " ";
	}

	LOG("bridge", "Prijata prace obsahuje:" + ss.str());

	game_.loadWork(moves);
}

std::vector<int> Bridge::getSolution()
{
	return std::vector<int>();
}

std::vector<std::vector<int>> Bridge::getWork(int count)
{
	return game_.getWork(count);
}

int Bridge::getBestResult()
{
	return 666;
}

void Bridge::setBestResult(int steps)
{

}

EWORK Bridge::work()
{
	return game_.run();
}


Bridge::Bridge()
{
}


Bridge::~Bridge()
{
}
