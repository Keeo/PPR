#include "stdafx.h"
#include "Board.h"


Board::Board()
{
}


Board::~Board()
{
}


void Board::setMove(Node* n)
{
	++cmoves_;
	std::vector<Node*> n_friends = *n->getFriends();
	for (auto &n_friend : n_friends) {
		graph_.removeNode(n_friend);
		Step s(n_friend, cmoves_);
		steps_.push_back(s);
	}
	Step s(n, cmoves_);
	graph_.removeNode(n);
	steps_.push_back(s);
}


bool const* Board::getAvailableMoves()
{
	return graph_.getNodeMask();
}


void Board::restoreLastMove()
{
	if (steps_.size() == 0) {
		std::cout << "[ERR] No moves to restore!" << std::endl;
		return;
	}

	Step* last = &steps_.back();
	while (last->move == cmoves_) {
		graph_.restoreNode(last->node);
		steps_.pop_back();
		if (steps_.size() == 0) {
			break;
		}
		else {
			last = &steps_.back();
		}
	}
	--cmoves_;
}


unsigned int Board::getCMoves()
{
	return cmoves_;
}


Graph& Board::getGraph()
{
	return graph_;
}

