#pragma once

#ifndef EXPERIMENT
#define EXPERIMENT

#include <map>
#include <vector>
#include "Configuration.h"
#include "Pool.h"

using namespace std;

//Experiment for genetic algorithm 

class Experiment
{
public:

	//NEAT experiment class
	//experiment configuration
	Configuration configuration;
	

	map<string, double> probabilities; //probabilities for mutations

	// link
	// bias
	// node
	// expressing
	// step
	// offset_range


	
	vector<Pool> pools;

	

	Experiment();
	Experiment(Configuration config);

	~Experiment();

	void init();


private:



};



#endif // !EXPERIMENT
