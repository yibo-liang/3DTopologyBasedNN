#include "../stdafx.h"
#include "Gene.h"



Gene Gene::copy()
{
	Gene c;
	c.innovation = this->innovation;
	c.node_in = this->node_in;
	c.node_out = this->node_out;
	c.weight = this->weight;
	for (int i = 0; i < this->offset_vector.size(); i++) {
		c.offset_vector.push_back(this->offset_vector[i]);
	}

	return c;
}

Gene::Gene()
{
}

Gene::~Gene()
{
}
