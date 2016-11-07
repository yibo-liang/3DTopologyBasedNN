

#include "Genome.h"

#include "Experiment.h"

#include <map>

using namespace std;

Experiment::Experiment()
{
	// link
	// bias
	// node
	// expressing
	// step
	// offset_range
	this->pool_size = 1;
	this->probabilities["link"] = 0.02;
	this->probabilities["bias"] = 0.02;
	this->probabilities["node"] = 0.02;
	this->probabilities["expressing"] = 0.02;
	this->probabilities["step"] = 0.02;
	this->probabilities["offset_range"] = 0.02;

}

Experiment::Experiment(int pool_size, int pool_population)
{
	Experiment();
	this->pool_population = pool_population;
	this->pool_size = pool_size;
}

Experiment::Experiment(int pool_size, int pool_population,  map<string, double> probabilities)
{
	Experiment();
	this->pool_population = pool_population;
	this->pool_size = pool_size;
	typedef map<string, double>::iterator it_type;
	//deep copy rather than assignment
	for (it_type iterator = probabilities.begin(); iterator != probabilities.end(); iterator++) {
		string key = iterator->first;
		double value = iterator->second;
		this->probabilities[key] = value;
	}
}

Experiment::~Experiment()
{
}

void Experiment::init(int input_n, int output_n)
{
	for (int i = 0; i < this->pool_size; i++) {
		Pool npool;
		for (int j = 0; j < this->pool_population; j++) {
			Genome ngenome=basicGenome(this->input_n);
			npool.addToSpecies(ngenome);
		}
		npool.innovation = 1;
		this->pools.push_back(npool);
	}
}
