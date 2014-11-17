#include "stdafx.h"
#include "Game.h"


Game::Game()
{
	cmoves_ = 500;
}


Game::~Game()
{
}


void Game::run()
{
	unsigned int depth = 0;
	
	while (stack_.size() > 0) {
		LOG("stack", "" + std::to_string(stack_.size()));
		Step step = stack_.back();
		stack_.pop_back();

		if (step.move < depth) {
			board_.restoreLastMove(depth - step.move);
			depth -= depth - step.move;
		}
		
		board_.setMove(step.node);
		board_.getGraph().print();
		if (board_.isWin()){
			save(board_.getCMoves(), board_.getSteps());
		}
		++depth;
		addNextMoves(depth);
	}
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


Graph& Game::getGraph()
{
	return board_.getGraph();
}


Board& Game::getBoard()
{
	return board_;
}

