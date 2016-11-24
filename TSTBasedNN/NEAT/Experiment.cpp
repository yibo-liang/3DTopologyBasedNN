

#include "Genome.h"

#include "Experiment.h"

#include <map>

using namespace std;


Experiment::Experiment()
{
	this->configuration = Configuration();
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
			Genome ngenome(this->configuration);
			npool.addToSpecies(ngenome);
		}
		this->pools.push_back(npool);
	}
}