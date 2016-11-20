#pragma once

#ifndef POOL
#define POOL

#include <vector>
#include "globals.h"
#include "Genome.h"
#include "Species.h"

class Pool
{
public:
	//list of species, remember to delete every single of them when distructing
	vector<Species> species;
	int generation;
	int resetInnovation();
	int getInnovation();

	

	void newGeneration();

	Pool();
	~Pool();

	void addToSpecies(Genome& genome);
private:
	int innovation;

};


Genome tournament_selection(vector<Genome>& genomes);

#endif // !POOL
