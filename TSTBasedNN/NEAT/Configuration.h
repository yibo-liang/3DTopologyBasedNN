#pragma once

#ifndef CONFIG
#define CONFIG

#include <map>
#include <string>

using namespace std;

class Configuration {
public:

	int pool_quantity;
	int pool_population;
	int input_n;
	int output_n;
	int space_dimension;
	int max_neuron;

	map<string, double> probabilities;

	Configuration();
	Configuration(int input_n, int output_n);
	~Configuration();

};

#endif // !CONFIG
