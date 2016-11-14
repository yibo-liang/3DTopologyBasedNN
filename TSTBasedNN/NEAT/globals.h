#pragma once

namespace constants {
	const int offset_input_neuron_id = 0;
	const int offset_bias_neuron_id = 10000;
	const int offset_hidden_neuron_id = 20000;
	const int offset_output_neuron_id = 30000;

	const int BIAS_NEURON = 0;
	const int INPUT_NEURON = 1;
	const int HIDDEN_NEURON = 2;
	const int OUTPUT_NEURON = 3;

	int get_id_offset(int type);
}