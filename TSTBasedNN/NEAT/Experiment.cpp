

#include "Genome.h"

#include "Configuration.h"
#include "Experiment.h"

#include <map>

using namespace std;


Experiment::Experiment()
{
}

Experiment::Experiment(Configuration config)
{
	this->configuration = config;
}


Experiment::~Experiment()
{
}

void Experiment::init()
{
	for (int i = 0; i < this->configuration.pool_quantity; i++) {
		Pool npool;
		for (int j = 0; j < this->configuration.pool_population; j++) {
			Genome* ngenome=basicGenome(this->configuration);
			npool.addToSpecies(ngenome);
		}
		this->pools.push_back(npool);
	}
}

Network Experiment::generateNeuralNetwork(Genome genome)
{
	Network * network = new Network();
	Configuration config = this->configuration;

	for (int i = 0; i < genome.configuration.input_n; i++) {
		Node * node = new Node();
		node->id = i;
		node->type = "input";
		network->nodes[i] = *node;

	}
	

	return *network;
}
