// 3DTopologyBasedNN.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NEAT\globals.h"
#include "NEAT\Preset.h"
#include "NEAT\Configuration.h"
#include "NEAT\Pool.h"

using namespace constants;
int main()
{
	vector<double> v1 = { 0,0,0 };
	vector<double> v2 = { 0,5,0 };
	vector<double> v3 = { 5,5,0 };

	constants::init_random();
	Preset preset;
	preset.add_t_gene(0, -1, INPUT_NEURON, v1);
	preset.add_t_gene(0, -1, OUTPUT_NEURON, v1);
	Configuration config;
	config.pool_population = 60;
	config.load_preset(preset);
	double PI = 3.141592653589793;
	Pool pool;

	vector<vector<double>> inputs_wrap;
	vector<vector<double>> outputs_wrap;
	for (double i = 0; i < 2*PI; i += PI / 100) {
		vector<double> inputs;
		vector<double> outputs;
		inputs.push_back(i);
		outputs.push_back(cos(i));
		inputs_wrap.push_back(inputs);
		outputs_wrap.push_back(outputs);
	}
	pool.configuration = config;
	pool.set_inputs(inputs_wrap);
	pool.set_outputs(outputs_wrap);

	for (int i = 0; i < 1; i++) {
		Genome g(config);
		g.init();
		pool.addToSpecies(g);
	}
	for (int i = 0; i < 200; i++) {
		pool.newGeneration();
	}


    return 0;

}

