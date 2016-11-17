#pragma once

#ifndef CONFIG
#define CONFIG

#include "Preset.h"
#include <map>
#include <list>
#include <string>
#include <vector>

using namespace std;
//structure to define preset neurons, such as input neuron 
//type, activation function, 
//The preset is solely designed to verify this TSTBasedNN, using existing successful network structure


class Configuration {
public:

	int pool_quantity;
	int pool_population;
	int input_n;
	int output_n;
	int bias_n;
	int space_dimension;
	int max_hidden_n;

	int offset_input_neuron_id;
	int offset_hidden_neuron_id;
	int offset_output_neuron_id;
	int offset_bias_neuron_id;

	double min_neuron_distance;

	map<string, double> probabilities;

	bool is_recurrent;

	double initial_weight_range;

	double disposition_weight;
	double disjoint_weight;
	double dweight_weight;

	bool is_preset_id(int id);

	int get_node_type_from_id(int id);

	Preset preset;

	Configuration();
	Configuration(const Configuration& obj);
	Configuration(int input_n, int output_n);
	~Configuration();
	void load_preset(const Preset& preset);

};

#endif // !CONFIG
