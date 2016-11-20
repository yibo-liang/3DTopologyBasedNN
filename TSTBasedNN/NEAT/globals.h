#pragma once

#ifndef CONSTANTS
#define CONSTANTS

#include <ctime>
#include <random>
#include <iostream>
namespace constants {
	const int offset_input_neuron_id = 0;
	const int offset_bias_neuron_id = 10000;
	const int offset_hidden_neuron_id = 20000;
	const int offset_output_neuron_id = 30000;

	const int BIAS_NEURON = 0;
	const int INPUT_NEURON = 1;
	const int HIDDEN_NEURON = 2;
	const int OUTPUT_NEURON = 3;

	static int get_id_offset(int type) {
		if (type == INPUT_NEURON) return offset_input_neuron_id;
		if (type == OUTPUT_NEURON) return offset_output_neuron_id;
		if (type == BIAS_NEURON) return offset_bias_neuron_id;
		if (type == HIDDEN_NEURON) return offset_hidden_neuron_id;
		std::cout << "Neuron Type Error: " << type;
		return 0;
	};

	const double MIN_EDGE_LENGTH = 0.5;// The minimum length that an edge will be treated, that is, if an edge length is 0.1, will be regarded as 0.5 in calculation
	//this is for the reason that if there is a sequence of very short edge, a signal might be travelling through multiple node in single time step, which defy the definition of single 1 time step

	static double random() {
		unsigned int time_ui = unsigned int(time(NULL));
		srand(time_ui);
		return ((double)rand() / (RAND_MAX));
	}


}
#endif // !CONSTANTS
