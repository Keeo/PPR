#pragma once
#include "stdafx.h"


class Node
{
	int node_number_;
	std::vector<Node*> friends_;

public:
	Node();
	Node(int node_number, int cfriends);

	void init(int node_number, int cfriends = 0);
	void addFriend(Node* node);
	Node* hasFriend(int node_number);
	void removeFriend(Node* node);
	void removeFriend(int pos);
	~Node();

	int getNodeNumber() const;

	bool operator == (const Node &that) const {
		return getNodeNumber() == that.getNodeNumber();
	}
};

