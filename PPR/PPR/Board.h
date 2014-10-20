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

	void restoreLastMove(int moves = 1);

	unsigned int getCMoves();

	Graph& getGraph();

	bool isWin();

	std::vector<Step> const* getSteps();

	Board();
	~Board();
};

