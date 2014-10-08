#pragma once

#include "Node.h"

class Graph
{
	Node* nodes_;
	bool* removed_nodes_;

	int cnodes_;

public:
	Graph();
	~Graph();

	void removeNode(Node* node);
	void restoreNode(Node* node);

	bool const* getNodeMask();

	void load(char* filepath);
	void print();

	Node* getNode(int node_number);
};

