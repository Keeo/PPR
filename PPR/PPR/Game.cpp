#include "stdafx.h"
#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
}


void Game::load(char* filepath)
{
	std::string line;
	std::ifstream graf(filepath);
	if (!graf.is_open())
	{
		std::cout << "[ERR] File cannot be opened!";
		return;
	}

	getline(graf, line);
	cnodes_ = std::stoi(line);
	nodes_ = new Node[cnodes_];

	for (int i = 0; i < cnodes_; ++i) {
		std::cout << "[INF] Processing node " << i << " friends: ";
		nodes_[i].init(i);
		getline(graf, line);
		for (std::string::size_type j = 0; j < line.size(); ++j) {
			if (line[j] == '1') {
				std::cout << j << ", ";
				nodes_[i].addFriend(&nodes_[j]);
			}
		}
		std::cout << std::endl;
	}
	graf.close();
}


void Game::print()
{
	std::cout << "[INF] Printing nodes..." << std::endl;
	std::cout << "Nodes count: " << cnodes_ << std::endl;
	for (int i = 0; i < cnodes_; ++i) {
		for (int j = 0; j < cnodes_; ++j) {
			std::cout<< (nodes_[i].hasFriend(j) != 0) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "[INT] Nodes printed.";
}

