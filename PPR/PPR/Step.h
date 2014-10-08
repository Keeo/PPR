#pragma once

#include "Node.h"

struct Step
{
	Node* node;
	unsigned int move;
	
	Step(Node* n, unsigned int m) : node(n), move(m)
	{
	}
};

