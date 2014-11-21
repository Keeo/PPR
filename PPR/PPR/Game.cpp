#include "stdafx.h"
#include "Game.h"


Game::Game()
{
	cmoves_ = 500;
}


Game::~Game()
{
}


void Game::init()
{
	unsigned int bp = board_.getGraph().getBreakablePoint();
	stack_.setBreakableDepth(bp);
}


EWORK Game::run()
{
	int looper = 200;

	while (stack_.size() > 0) {
		LOG("stack", "" + std::to_string(stack_.size()));
		LOG("stack", stack_.getString());


		unsigned int depth = board_.getCMoves();
		Step step = stack_.back();
		stack_.pop_back();

		if (step.move < depth) {
			board_.restoreLastMove(depth - step.move);
		}
		
		board_.setMove(step.node);
		board_.getGraph().print();
		if (board_.isWin()){
			save(board_.getCMoves(), board_.getSteps());
		}
		addNextMoves(board_.getCMoves());
		if (--looper == 0) {
			break;
		}
	}

	return stack_.size() == 0 ? EWORK_OUT_OF_WORK : EWORK_OK;
}


void Game::save(unsigned int cmoves, std::vector<Step> const* steps)
{
	if (cmoves < cmoves_) {
		cmoves_ = cmoves;
		steps_ = *steps;
		LOG("win", "New win - " + std::to_string(board_.getCMoves()));
	}
}


void Game::addNextMoves(unsigned int depth)
{
	for (auto node : board_.getGraph().getGraphIterator()) {
		Step s(&*node, depth);
		stack_.push_back(s);
	}
}


void Game::loadWork(std::vector<int> moves)
{
	stack_.loadWork(moves, &board_);
	addNextMoves(board_.getCMoves());
}


std::vector<int> Game::getWork()
{
	return stack_.getWork(board_.getSteps());
}


Graph& Game::getGraph()
{
	return board_.getGraph();
}


Board& Game::getBoard()
{
	return board_;
}


Stack& Game::getStack()
{
	return stack_;
}


void Game::setBestSolutionsMoves(int moves)
{
	cmoves_ = moves;
	steps_.clear();
}


int Game::getBestSolutionsMoves()
{
	return cmoves_;
}


std::vector<int> Game::getWinningSequence()
{
	std::vector<Step> moves = Board::transformStepsToMove(&steps_);
	std::vector<int> out;
	for (auto &i : moves) {
		out.push_back(i.node->getNodeNumber());
	}
	return out;
}

