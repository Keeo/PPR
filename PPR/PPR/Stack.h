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

	std::vector<int> getOneWork(const std::vector<Step> boardSteps);
	bool isForkable(const Step& s);


public:
	void setBreakableDepth(int depth);

	std::vector<std::vector<int>> getWork(int count, const std::vector<Step> boardSteps);
	void loadWork(std::vector<int>& moves, Board* b);
};

