#pragma once

struct GraphIterator;

#include "Node.h"

class Graph
{
	Node* nodes_;
	bool* removed_nodes_;

	unsigned int cnodes_;
	unsigned int cremoved_nodes_;

public:
	Graph();
	~Graph();

	void removeNode(Node* node);
	void restoreNode(Node* node);

	bool const* getRemovedNodes();

	void load(char* filepath);
	void print();

	Node* getNode(int node_number);
	unsigned int getCNodes();
	unsigned int getCRemovedNodes();
	GraphIterator getGraphIterator();
};

