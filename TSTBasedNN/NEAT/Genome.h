#pragma once

#ifndef GENOME
#define GENOME

#include "Configuration.h"
#include "Gene.h"
#include <map>
#include <vector>
#include "../NN/Network.h"

using namespace std;

const int BIAS=0;
const int INPUT = 1;
const int HIDDEN = 2;
const int OUTPUT = 3;
class Genome
{
public:

	vector<Gene> genes;
	double fitness;
	double adjusted_fitness;
	Network network;

	Configuration configuration;
	int hidden_n;
	
	double disjointCompare(Genome g1, Genome g2);
	double weightCompare(Genome g1, Genome g2);
	double dispositionCompare(Genome g1, Genome g2);

	
	bool isSameSpecies(Genome another);
	

	Genome(Configuration configuration);
	~Genome();

	Genome copy();
	Network toNeuralNetwork();
	int randomNeuron(bool inclInput, bool inclBias);
	bool containsGene(Gene gene);

private:

	map<int, int> neurons;
};

Genome* basicGenome(Configuration config);
//mutate method with innovation function
Genome fromMutate(Genome g, int (*inno_func)());
Genome fromCrossOver(Genome g1, Genome g2, map<string, double> probabilities);



#endif // !GENOME
