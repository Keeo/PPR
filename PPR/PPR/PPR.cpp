#include "stdafx.h"
#include "Game.h"


int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cout << "[INF] Run with graph set: ./ppr ../../Assets/n10k5.grf";
		return 1;
	}

	Game game;
	Graph& gm = game.getGraph();
	gm.load(argv[1]);
	gm.print();

	Node* n2 = gm.getNode(2);
	Node* n3 = gm.getNode(3);
	Node* n4 = gm.getNode(4);
	
	gm.removeNode(n2);
	gm.removeNode(n3);
	gm.removeNode(n4);

	gm.print();

	gm.restoreNode(n2);
	gm.restoreNode(n3);
	gm.restoreNode(n4);

	gm.print();


	std::cin.get();
	return 0;
}

