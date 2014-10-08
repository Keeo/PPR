#pragma once

#include "Node.h"
#include "Graph.h"
#include "Board.h"
#include "Step.h"

class Board
{
	std::vector<Step> steps_;
	
	unsigned int cmoves_;

	Graph graph_;

public:

	void setMove(Node* n);

	bool const* getAvailableMoves();

	void restoreLastMove();

	unsigned int getCMoves();

	Graph& getGraph();

	Board();
	~Board();
};

