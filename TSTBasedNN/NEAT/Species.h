#pragma once

#ifndef SPECIES
#define SPECIES

#include <vector>
#include "Genome.h"

using namespace std;

class Species
{
public:
	double top_fitness;
	int staleness;
	//list of genomes, remember to delete every single one, when destructing
	vector<Genome> genomes;

	Species(const Species& obj);
	Species();
	~Species();

private:

};

#endif // !SPECIES
