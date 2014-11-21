#include "stdafx.h"
#include "Graph.h"
#include "GraphIterator.h"


Graph::Graph() : cremoved_nodes_(0)
{
}


Graph::~Graph()
{
	delete[] nodes_;
	delete[] removed_nodes_;
}


void Graph::load(std::istream* stream)
{
	std::string line;

	getline(*stream, line);
	cnodes_ = std::stoi(line);
	nodes_ = new Node[cnodes_];
	removed_nodes_ = new bool[cnodes_];

	for (unsigned int i = 0; i < cnodes_; ++i) {
		std::stringstream ss;
		ss << "Processing node " << i << " friends: ";
		nodes_[i].init(i);
		getline(*stream, line);
		for (std::string::size_type j = 0; j < line.size(); ++j) {
			if (line[j] == '1') {
				ss << j << ", ";
				nodes_[i].addFriend(&nodes_[j]);
			}
		}
		removed_nodes_[i] = false;
		LOG("inf", ss.str());
	}
}


void Graph::print()
{
#ifdef _DEBUG
	LOG("inf", "Printing nodes...");
	LOG("inf", "Nodes count: " + std::to_string(cnodes_));
	for (unsigned int i = 0; i < cnodes_; ++i) {
		if (removed_nodes_[i]) {
			continue;
		}
		std::stringstream ss;
		ss << i << " - ";
		for (unsigned int j = 0; j < cnodes_; ++j) {
			if (removed_nodes_[j]) {
				continue;
			}
			ss << (nodes_[i].hasFriend(j) != 0) << " ";
		}
		LOG("node", ss.str());
	}
	LOG("inf", "Nodes printed");
#endif
}


void Graph::removeNode(Node* node)
{
	LOG("inf", "Removing node " + std::to_string(node->getNodeNumber()) + ".");
	if (removed_nodes_[node->getNodeNumber()]) {
		LOG("graph", "Removing removed node!");
		exit(67);
	}

	node->removeMyselfFromFriends();
	removed_nodes_[node->getNodeNumber()] = true;
	++cremoved_nodes_;
}


void Graph::restoreNode(Node* node)
{
	LOG("inf", "Restoring node " + std::to_string(node->getNodeNumber()) + ".");
	if (!removed_nodes_[node->getNodeNumber()]) {
		LOG("graph", "Restoring restored node!");
		exit(67);
	}

	node->addMyselfToFriends();
	removed_nodes_[node->getNodeNumber()] = false;
	--cremoved_nodes_;
}


unsigned int Graph::getAverageDegree()
{
	int k = 0;
	for (unsigned int i = 0; i < cnodes_; ++i) {
		k += nodes_[i].getCFriends();
	}
	return k / cnodes_;
}


unsigned int Graph::getBestPossibleSolution()
{
	return cnodes_ / (getAverageDegree() + 1);
}


unsigned int Graph::getWorstPossibleSolution()
{
	return cnodes_ - getAverageDegree();
}


unsigned int Graph::getBreakablePoint()
{
	return std::max(0, (int)cnodes_ - 12);
}


Node* Graph::getNode(int node_number)
{
	return &nodes_[node_number];
}


bool const* Graph::getRemovedNodes()
{
	return removed_nodes_;
}


inline unsigned int Graph::getCNodes()
{
	return cnodes_;
}


inline unsigned int Graph::getCRemovedNodes()
{
	return cremoved_nodes_;
}


GraphIterator Graph::getGraphIterator()
{
	return GraphIterator(this);
}

