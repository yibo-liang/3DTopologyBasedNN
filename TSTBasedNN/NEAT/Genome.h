#pragma once

#ifndef GENOME
#define GENOME

#include "Configuration.h"
#include "Gene.h"
#include <map>
#include <vector>
#include "../NN/Network.h"

using namespace std;



class Genome
{
public:

	vector<Gene> genes;
	double fitness;
	double adjusted_fitness;
	Network network;

	Configuration configuration;

	int input_n;
	int hidden_n;
	int output_n;
	int max_neuron;
	
	double disjointCompare(Genome g1, Genome g2);
	double weightCompare(Genome g1, Genome g2);
	double dispositionCompare(Genome g1, Genome g2);

	
	bool isSameSpecies(Genome another);
	

	Genome(Configuration configuration);
	~Genome();

	Network toNeuralNetwork();

private:

};

Genome* basicGenome(Configuration config);
//mutate method with innovation function
Genome fromMutate(Genome g, map<string, double> probabilities, int (*inno_func)(int));
Genome fromCrossOver(Genome g1, Genome g2, map<string, double> probabilities);



#endif // !GENOME
