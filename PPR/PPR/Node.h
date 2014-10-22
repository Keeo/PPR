#pragma once
#include "stdafx.h"


class Node
{
	int node_number_ = -1;
	std::vector<Node*> friends_;

public:
	Node();
	~Node();

	Node(int node_number);

	void init(int node_number);

	void addFriend(Node* node);
	void addMyselfToFriends();

	Node* hasFriend(int node_number);

	void removeFriend(Node* node);
	void removeMyselfFromFriends();
	
	// Content of vector will change based on called methods.
	std::vector<Node*>* getFriends();
	
	int getNodeNumber() const;

	bool operator == (const Node &that) const {
		return getNodeNumber() == that.getNodeNumber();
	}
};

