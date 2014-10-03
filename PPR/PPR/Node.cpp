#include "stdafx.h"
#include "Node.h"


Node::Node()
{
}


Node::Node(int node_number, int cfriends)
{
	init(node_number, cfriends);
}


inline void Node::init(int node_number, int cfriends)
{
	node_number_ = node_number;
	if (cfriends != 0){
		friends_.reserve(cfriends);
	}
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
		std::cout << "[WRN] Trying to remove non existing node!";
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

