#pragma once

#ifndef GENOME
#define GENOME

#include "Gene.h"
#include <map>
#include "../3DNN/Network.h"

using namespace std;



class Genome
{
public:
	
	map<int, Gene> genes;
	double fitness;
	double adjusted_fitness;
	Network network;
	int max_neuron;
	
	double disjointCompare(Genome g1, Genome g2);
	double weightCompare(Genome g1, Genome g2);
	double dispositionCompare(Genome g1, Genome g2);

	
	bool isSameSpecies(Genome another);
	

	Genome();
	~Genome();

private:

};

Genome basicGenome(int max_neuron);
Genome fromMutate(Genome g, map<string, double> probabilities);
Genome fromCrossOver(Genome g1, Genome g2, map<string, double> probabilities);
#endif // !GENOME
