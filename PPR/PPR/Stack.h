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

	bool isForkable(const Step& s);

public:
	void setBreakableDepth(int depth);

	std::vector<int> getWork(std::vector<Step> const* boardSteps);
	void loadWork(std::vector<int>& moves, Board* b);
	std::string getString();
};

