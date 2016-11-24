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
	Configuration configuration;

	double adoption_rate = 0.01;

	Genome(*GASelection)(vector<Genome>);//Selection function for Genetic Algorithm

	void newGeneration();
	void addToSpecies(Genome& genome);

	Pool(const Pool& obj);
	Pool();
	~Pool();

private:
	int innovation;
	vector<vector<double>> inputs;
	vector<vector<double>> outputs;
	int genome_belong_to(Genome& genome);
};


#endif // !POOL
