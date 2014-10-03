#include "stdafx.h"
#include "Node.h"


Node::Node()
{
}


Node::Node(int node_number, int cfriends) :
node_number_(node_number),
friends_(cfriends)
{
}


void Node::init(int node_number, int cfriends)
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


void Node::removeFriend(int pos)
{
	friends_[pos] = 0;
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

