#include "stdafx.h"
#include "Stack.h"


std::vector<std::vector<int>> Stack::getWork(int count, std::vector<Step> const* boardSteps)
{
	std::vector<std::vector<int>> ret(count);

	for (int i = 0; i < count; ++i) {
		ret[i] = getOneWork(boardSteps);
	}
	
	return ret;
}


std::vector<int> Stack::getOneWork(std::vector<Step> const* boardSteps)
{
	LOG("stack", "Building one work");
	std::vector<int> out;
	if (!isForkable(front())) {
		return out;
	}
	LOG("stack", "Forkable: true");
	
	for (auto &a : *boardSteps) {
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

