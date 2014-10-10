#pragma once

#include <iterator>
#include "Graph.h"
#include "Node.h"

struct GraphIterator : public std::iterator<std::input_iterator_tag, Node*>
{

	GraphIterator(Graph* graph, int position = 0);

	GraphIterator& operator++();

	GraphIterator begin();

	GraphIterator end();

	Node* operator*();
	bool operator==(const GraphIterator&);
	bool operator!=(const GraphIterator&);

private:
	Graph* graph_;
	unsigned int position_;
};

