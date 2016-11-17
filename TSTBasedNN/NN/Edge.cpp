
#include "Edge.h"

Edge::Edge(const Edge & obj)
{
	this->id = obj.id;
	this->length = obj.length;
	this->node_in = obj.node_in;
	this->node_out = obj.node_out;
	for (auto it = obj.transmitting_signals.begin(); it!=obj.transmitting_signals.end(); it++) {
		this->transmitting_signals.push_back(*it);
	}
}

Edge::Edge()
{
}

Edge::~Edge()
{
}
