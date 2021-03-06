#include "stdafx.h"
#include "Node.h"


Node::Node()
{
}


Node::Node(int node_number)
{
	init(node_number);
}


inline void Node::init(int node_number)
{
	node_number_ = node_number;
}


Node::~Node()
{
}


void Node::addFriend(Node* node)
{
	friends_.push_back(node);
}


void Node::removeFriend(Node* node)
{
	auto it = std::find(friends_.begin(), friends_.end(), node);
	if (it != friends_.end()) {
		friends_.erase(it);
	}
	else {
		LOG("wrn", "Trying to remove non existing node!");
	}
}


Node* Node::hasFriend(int node_number)
{
	for (auto &i : friends_) {
		if (i->getNodeNumber() == node_number) {
			return i;
		}
	}
	return 0;
}


int Node::getNodeNumber() const
{
	return node_number_;
}


std::vector<Node*>* Node::getFriends()
{
	return &friends_;
}


void Node::removeMyselfFromFriends()
{
	for (auto &i : friends_) {
		i->removeFriend(this);
	}
}


void Node::addMyselfToFriends()
{
	for (auto &i : friends_) {
		i->addFriend(this);
	}
}

