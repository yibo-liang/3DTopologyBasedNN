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
	
	
	vector<Pool> pools;

	

	Experiment();
	Experiment(Configuration config);

	~Experiment();

	void init();


private:



};



#endif // !EXPERIMENT
