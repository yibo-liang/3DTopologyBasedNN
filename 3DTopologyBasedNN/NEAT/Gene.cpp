#include "../stdafx.h"
#include "Gene.h"



Gene Gene::copy()
{
	Gene c;
	c.innovation = this->innovation;
	c.node_in = this->node_in;
	c.node_out = this->node_out;
	c.weight = this->weight;

	c.offset_x = this->offset_x;
	c.offset_y = this->offset_y;
	c.offset_z = this->offset_z;

	return c;
}

Gene::Gene()
{
}

Gene::~Gene()
{
}
