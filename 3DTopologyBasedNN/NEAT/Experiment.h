#pragma once

#ifndef EXPERIMENT
#define EXPERIMENT

#include <map>
#include <list>
#include "Pool.h"

using namespace std;



class Experiment
{
public:

	//NEAT experiment class
	//experiment configuration
	int pool_size;
	int pool_population;
	
	int input_n;
	int output_n;

	map<string, double> probabilities; //probabilities for mutations

	// link
	// bias
	// node
	// expressing
	// step
	// offset_range


	
	list<Pool> pools;

	


	Experiment();

	Experiment(int pool_size, int pool_population);

	Experiment(int pool_size, int pool_population, map<string, double> probabilities);

	~Experiment();

	void init(int input_n, int output_n);

private:



};


#endif // !EXPERIMENT
