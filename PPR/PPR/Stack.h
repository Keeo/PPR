#pragma once

#include "stdafx.h"
#include "Step.h"
#include "limits.h"
#include <deque>
#include "Board.h"

class Stack : public std::deque<Step>
{
	int breakableDepth_ = INT_MAX;

	int getcForkable();

	std::vector<int> getOneWork(std::vector<Step> const* boardSteps);
	bool isForkable(const Step& s);


public:
	void setBreakableDepth(int depth);

	std::vector<std::vector<int>> getWork(int count, std::vector<Step> const* boardSteps);
	void loadWork(std::vector<int>& moves, Board* b);
};

