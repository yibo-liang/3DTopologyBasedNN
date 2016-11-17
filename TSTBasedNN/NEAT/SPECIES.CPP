
#include "Species.h"

Species::Species(const Species & obj)
{
	this->genomes = obj.genomes;
	this->staleness = obj.staleness;
	this->top_fitness = obj.top_fitness;
}

Species::Species()
{
}

Species::~Species()
{
}
