
#include "TGene.h"
#include "globals.h"

using namespace constants;
TGene::TGene(const TGene & obj)
{
	this->id = obj.id;
	this->base = obj.base;
	this->enabled = obj.enabled;
	/*for (int i = 0; i < obj.offset.size(); i++) {
		this->offset.push_back(obj.offset[i]);
	}*/
	this->fixed = obj.fixed;
	this->innovation = obj.innovation;
	this->offset = obj.offset;
	this->type = obj.type;
}

TGene::TGene()
{
	this->enabled = true;
	this->fixed = false;
	this->type = HIDDEN_NEURON;
}

TGene::~TGene()
{
}
