#pragma once

#ifndef POOL
#define POOL

#include <vector>
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


	Genome(*GASelection)(vector<Genome>);//Selection function for Genetic Algorithm

	void newGeneration();

	Pool();
	~Pool();

	void addToSpecies(Genome& genome);
private:
	int innovation;

};


#endif // !POOL
