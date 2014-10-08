#include "stdafx.h"
#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
}

Graph& Game::getGraph()
{
	return board_.getGraph();
}

Board& Game::getBoard()
{
	return board_;
}