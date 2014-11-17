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
	int intCount = (dataLength * sizeof(MPI_BYTE)) / sizeof(MPI_INT);
	std::vector<int> moves(temp, temp + intCount);
	
	LOG("bridge", "Size of dataLength: " + std::to_string(dataLength));
	LOG("bridge", "Size of received work: " + std::to_string(intCount));


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

std::vector<int> Bridge::getWork()
{
	return game_.getWork();
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
