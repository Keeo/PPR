#pragma once

struct GraphIterator;

#include "Node.h"

class Graph
{
	Node* nodes_;
	bool* removed_nodes_;

	unsigned int cnodes_;
	unsigned int cremoved_nodes_;

	unsigned int getAverageDegree();

public:
	Graph();
	~Graph();

	void removeNode(Node* node);
	void restoreNode(Node* node);

	bool const* getRemovedNodes();

	void load(std::istream* stream);
	void print();

	Node* getNode(int node_number);
	const unsigned int& getCNodes();
	const unsigned int& getCRemovedNodes();
	GraphIterator getGraphIterator();

	unsigned int getBestPossibleSolution();
	unsigned int getWorstPossibleSolution();
	unsigned int getBreakablePoint();
};

