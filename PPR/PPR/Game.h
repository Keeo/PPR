#pragma once

#include "Node.h"
#include "Graph.h"
#include "Board.h"
#include "Step.h"
#include "GraphIterator.h"

class Game
{
	Board board_;

	std::vector<Step> steps_;

	unsigned int cmoves_;

public:
	Game();
	~Game();

	void run();
	void addNextMovesToHistory(std::vector<Step>* history, unsigned int depth);

	void save(unsigned int csteps, std::vector<Step> const* steps);

	Graph& getGraph();
	Board& getBoard();
};

