
#include "Node.h"

Node::Node(const Node & obj)
{
	this->activation = obj.activation;
	this->edges_in = obj.edges_in;
	this->edges_out = obj.edges_in;
	this->activation_signals = obj.activation_signals;
	this->id = obj.id;
	this->position = obj.position;
	this->type = obj.type;
}

Node::Node()
{
	this->type = "hidden";
}

Node::~Node()
{
}
