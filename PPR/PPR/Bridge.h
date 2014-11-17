#pragma once
#include "Game.h"

class Bridge
{

	Game game_;

public:
	/* data je surove precteny soubor */
	void initData(char* data, int dataLength, bool start);
	
	/* Prace ziskana ze getWork(). */ 
	void setWork(char* data, int dataLength);

	/* Vzdy vrati vector s dimenzi v count. Vnitrni vectory mohou mit velikost 0 jako signalizaci ze pro tento procesor uz prace neni. */
	std::vector<std::vector<int>> getWork(int count);

	/* Vrati vyherni kroky. */
	std::vector<int> getSolution();

	/* Omezovani hloubky z nalezenych resenich. */
	int getBestResult();
	void setBestResult(int steps);

	/* Pracuje omezeny cas/pocet cyklu. Navratova hodnota rika jestli prace dosla. */
	EWORK work();

	Bridge();
	~Bridge();
};

