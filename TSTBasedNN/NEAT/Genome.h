#pragma once

#ifndef GENOME
#define GENOME


#include <map>
#include "Configuration.h"
#include "LGene.h"
#include "TGene.h"
#include <vector>
#include "../NN/Network.h"

using namespace std;

class Genome
{
public:

	int pool_id;

	vector<LGene> l_genes;
	vector<TGene> t_genes;

	double fitness;
	double shared_fitness;
	Network network;

	Configuration configuration;
	int hidden_n;
	int bias_n;

	//ptr of innovation function of the pool this genome blongs
	int inno_func();
	int reset_inno();

	double disjointCompare(Genome g1, Genome g2);
	double weightCompare(Genome g1, Genome g2);
	double dispositionCompare(Genome g1, Genome g2);


	bool isSameSpecies(const Genome& another);

	Genome(const Genome& obj);
	Genome(const Configuration& configuration);
	~Genome();

	void init();
	Network toNeuralNetwork();
	int randomNeuron(bool inclInput, bool inclBias);
	bool containsLGene(const LGene& lgene);
	TGene& get_tgene_by_id(int id);

private:

	//map<int, int> neurons;
};

//mutate method with innovation function
Genome fromMutate(const Genome &g);
Genome fromCrossOver(const Genome& g1, const Genome& g2);



#endif // !GENOME
