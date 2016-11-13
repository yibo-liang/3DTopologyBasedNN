#pragma once

#ifndef CONFIG
#define CONFIG

#include <map>
#include <list>
#include <string>
#include <vector>

using namespace std;

//structure to define preset neurons, such as input neuron 
//type, activation function, 
struct preset {
	int id;
	vector<double>  offset;
} ;

class Configuration {
public:
	
	int pool_quantity;
	int pool_population;
	int input_n;
	int output_n;
	int space_dimension;
	int max_neuron;

	int offset_input_neuron_id;
	int offset_hidden_neuron_id;
	int offset_output_neuron_id;

	map<int, preset> preset_input;
	map<int, preset> preset_hidden;
	map<int, preset> preset_output;

	map<string, double> probabilities;

	bool is_biased;
	bool is_recurrent;

	double initial_weight_range;

	double disposition_weight;
	double disjoint_weight;
	double dweight_weight;

	bool is_preset_id(int id);

	Configuration();
	Configuration(int input_n, int output_n);
	~Configuration();

};

#endif // !CONFIG
