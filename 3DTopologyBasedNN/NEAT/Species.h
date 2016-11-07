#pragma once

#ifndef SPECIES
#define SPECIES

#include <list>
#include "Genome.h"

using namespace std;

class Species
{
public:
	double top_fitness;
	int staleness;
	list<Genome> genomes;

	Species();
	~Species();

private:

};

#endif // !SPECIES
