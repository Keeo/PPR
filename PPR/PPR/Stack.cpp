#include "stdafx.h"
#include "Stack.h"


std::vector<std::vector<int>> Stack::getWork(int count, const std::vector<Step> boardSteps)
{
	std::vector<std::vector<int>> ret(count);

	for (int i = 0; i < count; ++i) {
		ret[i] = getOneWork(boardSteps);
	}
	
	return ret;
}


std::vector<int> Stack::getOneWork(const std::vector<Step> boardSteps)
{
	std::vector<int> out;
	if (!isForkable(front())) {
		return out;
	}

	for (auto &a : boardSteps) {
		if (a.move >= front().move) {
			break;
		}
		out.push_back(a.node->getNodeNumber());
	}

	out.push_back(front().node->getNodeNumber());
	pop_front();

	return out;
}


int Stack::getcForkable()
{
	int ret = 0;
	for (auto i = begin(); i != end(); ++i) {
		if (isForkable(*i)) {
			++ret;
		}
		else {
			break;
		}
	}
	return ret;
}


bool Stack::isForkable(const Step& step)
{
	return step.move < breakableDepth_;
}


void Stack::loadWork(std::vector<int>& moves, Board* board)
{
	board->reset();
	for (auto &i : moves) {
		board->setMove(i);
	}
}

