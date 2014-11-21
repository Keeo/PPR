#include "stdafx.h"
#include "Stack.h"


std::vector<int> Stack::getWork(std::vector<Step> const* boardSteps)
{
	LOG("stack", "Building one work deque size:" + std::to_string(size()));
	std::vector<int> out;
	if (size() == 0 || !isForkable(front())) {
		LOG("Stack", "Work split denied! Size:" + std::to_string(size()) + " isForkable:" + (size() > 0 ? std::to_string(isForkable(front())) : "n/a"));
		return out;
	}
	LOG("stack", "Forkable: true, depth comparasion front.move:" + std::to_string(front().move) + " depthLimit:" + std::to_string(breakableDepth_));
	
	LOG("stack", "Board contain: " + std::to_string(boardSteps->size()) + "steps.");
	std::vector<Step> cleanupSteps = Board::transformStepsToMove((std::vector<Step>*)boardSteps);
	LOG("stack", "Cleaned board contain: " + std::to_string(cleanupSteps.size()) + "steps.");
	for (auto &a : cleanupSteps) {
		if (a.move >= front().move) {
			break;
		}
		out.push_back(a.node->getNodeNumber());
	}
	LOG("stack", std::to_string(out.size()) + "steps used as history.");

	LOG("stack", "Reading node from beggining of stack, node id:" + std::to_string(front().node->getNodeNumber()));
	out.push_back(front().node->getNodeNumber());

	LOG("stack", "Removing node from stack.");
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


std::string Stack::getString()
{
	std::stringstream ss;
	ss << "Values in stack: ";
	for (auto& i : *this) {
		ss << "{" << i.node->getNodeNumber() << ", " << i.move << "} ";
	}
	return ss.str();
}


void Stack::setBreakableDepth(int depth)
{
	LOG("Stack", "Setting breakable depth: " + std::to_string(depth));
	breakableDepth_ = depth;
}

