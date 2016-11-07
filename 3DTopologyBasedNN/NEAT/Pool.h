#pragma once

#ifndef POOL
#define POOL

#include <list>
#include "Genome.h"
#include "Species.h"

class Pool
{
public:
	list<Species> species;
	int generation;
	int innovation;

	Pool();
	~Pool();

	void addToSpecies(Genome genome);
private:

};


#endif // !POOL
