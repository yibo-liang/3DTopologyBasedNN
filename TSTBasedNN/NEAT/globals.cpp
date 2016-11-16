
#include "globals.h"
#include <iostream>

using namespace constants;
int constants::get_id_offset(int type)
{
	if (type == INPUT_NEURON) return offset_input_neuron_id;
	if (type == OUTPUT_NEURON) return offset_output_neuron_id;
	if (type == BIAS_NEURON) return offset_bias_neuron_id;
	if (type == HIDDEN_NEURON) return offset_hidden_neuron_id;
	std::cout << "Neuron Type Error: " << type;
	return 0;
}
