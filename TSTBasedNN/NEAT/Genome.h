#pragma once

#ifndef GENOME
#define GENOME

#include "Configuration.h"
#include "LGene.h"
#include "TGene.h"
#include <map>
#include <vector>
#include "../NN/Network.h"

using namespace std;

class Genome
{
public:

	vector<LGene> l_genes;
	vector<TGene> t_genes;

	double fitness;
	double adjusted_fitness;
	Network network;

	Configuration configuration;
	int hidden_n;
	//ptr of innovation function of the pool this genome blongs
	int(*inno_func)();
	int(*reset_inno)();
	
	double disjointCompare(Genome g1, Genome g2);
	double weightCompare(Genome g1, Genome g2);
	double dispositionCompare(Genome g1, Genome g2);

	
	bool isSameSpecies(const Genome& another);
	

	Genome(const Configuration& configuration);
	~Genome();

	Genome copy();

	void init();
	Network toNeuralNetwork();
	int randomNeuron(bool inclInput, bool inclBias);
	bool containsLGene(const LGene& lgene);

private:

	//map<int, int> neurons;
};

//mutate method with innovation function
Genome fromMutate(Genome g);
Genome fromCrossOver(Genome g1, Genome g2, map<string, double> probabilities);



#endif // !GENOME
