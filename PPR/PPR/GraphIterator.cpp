#include "stdafx.h"
#include "GraphIterator.h"


GraphIterator::GraphIterator(Graph* graph, int position) :
graph_(graph), position_(position)
{}


GraphIterator& GraphIterator::operator++()
{
	do
	{
		++position_;
	} while (position_ < graph_->getCNodes() && graph_->getRemovedNodes()[position_]);

	return *this;
}


GraphIterator GraphIterator::begin()
{
	unsigned int pos;
	for (pos = 0; pos < graph_->getCNodes(); ++pos) {
		if (!graph_->getRemovedNodes()[pos]){
			break;
		}
	}
	return GraphIterator(graph_, pos);
}


GraphIterator GraphIterator::end()
{
	return GraphIterator(graph_, graph_->getCNodes());
}


Node* GraphIterator::operator*()
{
	return graph_->getNode(position_);
}


bool GraphIterator::operator==(const GraphIterator& that)
{
	return position_ == that.position_ && graph_ == that.graph_;
}


bool GraphIterator::operator!=(const GraphIterator& that)
{
	return position_ != that.position_ || graph_ != that.graph_;
}

