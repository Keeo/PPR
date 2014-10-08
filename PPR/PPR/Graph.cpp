#include "stdafx.h"
#include "Graph.h"


Graph::Graph()
{
}


Graph::~Graph()
{
}


void Graph::load(char* filepath)
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
	removed_nodes_ = new bool[cnodes_];

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
		removed_nodes_[i] = false;
		std::cout << std::endl;
	}
	graf.close();
}


void Graph::print()
{
	std::cout << "[INF] Printing nodes..." << std::endl;
	std::cout << "Nodes count: " << cnodes_ << std::endl;
	for (int i = 0; i < cnodes_; ++i) {
		if (removed_nodes_[i]) {
			continue;
		}
		std::cout << i << " - ";
		for (int j = 0; j < cnodes_; ++j) {
			if (removed_nodes_[j]) {
				continue;
			}
			std::cout << (nodes_[i].hasFriend(j) != 0) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "[INF] Nodes printed." << std::endl;
}


void Graph::removeNode(Node* node)
{
	std::cout << "Removing node " << node->getNodeNumber() << "." << std::endl;
	node->removeMyselfFromFriends();
	removed_nodes_[node->getNodeNumber()] = true;
}


void Graph::restoreNode(Node* node)
{
	std::cout << "Restoring node " << node->getNodeNumber() << "." << std::endl;
	node->addMyselfToFriends();
	removed_nodes_[node->getNodeNumber()] = false;
}


Node* Graph::getNode(int node_number)
{
	return &nodes_[node_number];
}


bool const* Graph::getNodeMask()
{
	return removed_nodes_;
}
