#pragma once

#include "Node.h"
#include "Graph.h"
#include "Board.h"
#include "Step.h"
#include "GraphIterator.h"
#include "Stack.h"

class Game
{
	Board board_;
	
	Stack stack_;

	std::vector<Step> steps_;

	unsigned int cmoves_;

public:
	Game();
	~Game();

	EWORK run();
	void addNextMoves(unsigned int depth);

	void save(unsigned int csteps, std::vector<Step> const* steps);

	void loadWork(std::vector<int> moves);
	std::vector<int> getWork();

	Graph& getGraph();
	Board& getBoard();
	Stack& getStack();
};

