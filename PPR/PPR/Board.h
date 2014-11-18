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

	void reset();

	void setMove(Node* n);

	void setMove(int node);

	void restoreLastMove(int moves = 1);

	unsigned int getCMoves();

	Graph& getGraph();

	bool isWin();

	std::vector<Step> const* getSteps();

	static std::vector<Step> transformStepsToMove(std::vector<Step>* step);

	Board();
	~Board();
};

