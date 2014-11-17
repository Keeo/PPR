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
	int intCount = dataLength / 4;
	
	std::vector<int> moves;
	moves.assign(temp, temp + intCount);

	LOG("bridge", "hm" + std::to_string(*temp));
	LOG("bridge", "Size of dataLength: " + std::to_string(dataLength));

	std::stringstream ss;
	for (auto &a : moves) {
		ss << a << " ";
	}
	
	LOG("bridge", "Prijata prace obsahuje:" + ss.str());

	game_.loadWork(moves);
}

std::vector<int> Bridge::getSolution()
{
	std::vector<int> out;
	for (auto &s : game_.getWinningSequence()) {
		out.push_back(s.node->getNodeNumber());
	}
	return out;
}

std::vector<int> Bridge::getWork()
{
	return game_.getWork();
}

int Bridge::getBestResult()
{
	return game_.getBestSolutionsMoves();
}

void Bridge::setBestResult(int steps)
{
	game_.setBestSolutionsMoves(steps);
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
