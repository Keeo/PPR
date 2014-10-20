#include "stdafx.h"
#include "Game.h"


int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cout << "[INF] Run with graph set: ./ppr ../../Assets/n10k5.grf";
		LOG("err", "Wrong parameters!");
		return 1;
	}
	Log::getInstance().setPrint(false);
	Game game;
	Graph& gm = game.getGraph();
	Board& board = game.getBoard();
	std::cout << argv[0];
	gm.load(argv[1]);
	gm.print();
	
	Node* n0 = gm.getNode(0);
	Node* n1 = gm.getNode(1);
	Node* n2 = gm.getNode(2);
	Node* n3 = gm.getNode(3);
	Node* n4 = gm.getNode(4);
	Node* n6 = gm.getNode(6);
	Node* n7 = gm.getNode(7);

	//board.setMove(n0);
	//board.setMove(n3);
	//board.setMove(n4);

	gm.print();

	game.run();
	gm.print();
	/*
	board.setMove(n0);
	board.setMove(n3);
	board.setMove(n4);
	board.setMove(n7);
	gm.print();

	std::cout << "[GAM] Player one " << (board.getCMoves() % 2 == 1 ? "wins." : "lose.") << std::endl;

	board.restoreLastMove();
	board.restoreLastMove();
	board.restoreLastMove();
	board.restoreLastMove();
	gm.print();*/

	std::cin.get();
	return 0;
}

