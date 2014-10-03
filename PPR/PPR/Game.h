#pragma once

#include "Node.h"

class Game
{
	Node* nodes_;
	int cnodes_;

public:
	Game();
	~Game();

	void print();
	void load(char* filepath);
};

