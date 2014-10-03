#include "stdafx.h"
#include "Game.h"


int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cout << "[INF] Run with graph set: ./ppr ../../Assets/n10k5.grf";
		return 1;
	}

	Game game;
	game.load(argv[1]);
	game.print();
	std::cin.get();
	return 0;
}

