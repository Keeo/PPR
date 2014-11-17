#pragma once
#include "Game.h"

class Bridge
{

	Game game_;

public:
	void initData(char* data, int dataLength, bool start);
	
	void setWork(char* data, int dataLength);

	std::vector<int> getWork();

	std::vector<int> getSolution();

	int getBestResult();
	void setBestResult(int steps);

	EWORK work();

	Bridge();
	~Bridge();
};

