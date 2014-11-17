#include "stdafx.h"
#include "Stack.h"


std::vector<std::vector<int>> Stack::getWork(int count, std::vector<Step> const* boardSteps)
{
	LOG("stack", "get work for:" + std::to_string(count) + "processors.");
	std::vector<std::vector<int>> ret(count);

	for (int i = 0; i < count; ++i) {
		ret[i] = getOneWork(boardSteps);
	}
	LOG("stack", "Work prepared.");
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
	
	LOG("stack", "Board contain: " + std::to_string(boardSteps->size()) + "steps.");
	for (auto &a : *boardSteps) {
		if (a.move >= front().move) {
			break;
		}
		out.push_back(a.node->getNodeNumber());
	}
	LOG("stack", std::to_string(out.size()) + "steps used as history.");

	out.push_back(front().node->getNodeNumber());
	pop_front();
	LOG("stack", "Data with work contains:" + std::to_string(out.size()) + "elements.");
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

