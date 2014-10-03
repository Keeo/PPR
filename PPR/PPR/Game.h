#pragma once

#include "Node.h"
#include "Graph.h"

class Game
{
	Graph Graph_;

public:
	Game();
	~Game();

	Graph& getGraph();
};

