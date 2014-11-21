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
	

	out.push_back(-1);
	int half = getNumberOfRootNodes() / 2;
	for (int i = 0; i < half; ++i) {
		LOG("stack", "Reading node from beggining of stack, node id:" + std::to_string(front().node->getNodeNumber()));
		out.push_back(front().node->getNodeNumber());
		LOG("stack", "Removing node from stack.");
		pop_front();
	}
	

	LOG("stack", "Data with work contains:" + std::to_string(out.size()) + "elements.");
	return out;
}


int Stack::getNumberOfRootNodes()
{
	int ret = 0;
	int root = -1;
	for (auto &i : *this) {
		if (root == -1) {
			root = i.move;
		}
		if (root != i.move) {
			break;
		}
		++ret;
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
	bool history = true;
	for (auto &i : moves) {
		if (history && i == -1) {
			history = false;
			continue;
		}

		if (history) {
			LOG("stack", "hraji krok:" + std::to_string(i));
			board->setMove(i);
		}
		else {
			LOG("stack", "pripravuji krok:" + std::to_string(i));
			Step step(board->getGraph().getNode(i), board->getCMoves());
			push_back(step);
		}
	}

	Step step = back();
	pop_back();
	
	board->setMove(step.node);
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

