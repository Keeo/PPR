#pragma once
#include "stdafx.h"


class Node
{
	int node_number_ = -1;
	std::vector<Node*> friends_;

public:
	Node();
	Node(int node_number, int cfriends = 0);

	void init(int node_number, int cfriends = 0);

	void addFriend(Node* node);
	void addMyselfToFriends();

	Node* hasFriend(int node_number);

	void removeFriend(Node* node);
	void removeMyselfFromFriends();

	~Node();

	int getNodeNumber() const;

	bool operator == (const Node &that) const {
		return getNodeNumber() == that.getNodeNumber();
	}
};

