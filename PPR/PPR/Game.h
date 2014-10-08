#pragma once

#include "Node.h"
#include "Graph.h"
#include "Board.h"

class Game
{
	Board board_;

public:
	Game();
	~Game();

	Graph& getGraph();
	Board& getBoard();
};

