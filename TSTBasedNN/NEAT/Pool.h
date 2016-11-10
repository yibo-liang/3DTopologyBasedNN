#pragma once

#ifndef POOL
#define POOL

#include <list>
#include "Genome.h"
#include "Species.h"

class Pool
{
public:
	//list of species, remember to delete every single of them when distructing
	list<Species> species;
	int generation;
	int newInnovation();

	

	void newGeneration();

	Pool();
	~Pool();

	void addToSpecies(Genome* genome);
private:
	int innovation;

};


#endif // !POOL
