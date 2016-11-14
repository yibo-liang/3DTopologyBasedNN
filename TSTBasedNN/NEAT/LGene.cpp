#include "../stdafx.h"
#include "LGene.h"



LGene::LGene(const LGene & obj)
{
	this->innovation = obj.innovation;
	this->node_in = obj.node_in;
	this->node_out = obj.node_out;
	this->weight = obj.weight;
	this->enabled = obj.enabled;

}

LGene::LGene()
{
}

LGene::~LGene()
{
}
