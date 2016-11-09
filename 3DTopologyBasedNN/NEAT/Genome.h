#pragma once

#ifndef GENOME
#define GENOME

#include "Configuration.h"
#include "Gene.h"
#include <map>
#include "../NN/Network.h"

using namespace std;



class Genome
{
public:
	
	map<int, Gene> genes;
	double fitness;
	double adjusted_fitness;
	Network network;

	int input_n;
	int output_n;
	int max_neuron;
	
	double disjointCompare(Genome g1, Genome g2);
	double weightCompare(Genome g1, Genome g2);
	double dispositionCompare(Genome g1, Genome g2);

	
	bool isSameSpecies(Genome another);
	

	Genome();
	~Genome();

	Network toNeuralNetwork();

private:

};

Genome* basicGenome(Configuration config);
Genome fromMutate(Genome g, map<string, double> probabilities);
Genome fromCrossOver(Genome g1, Genome g2, map<string, double> probabilities);
#endif // !GENOME
