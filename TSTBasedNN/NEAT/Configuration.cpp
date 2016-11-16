
#include "Configuration.h"


bool Configuration::is_preset_id(int id)
{
	return false;
}

int Configuration::get_node_type_from_id(int id)
{
	for (int i = 0; i < this->preset.preset_t.size(); i++) {
		if (this->preset.preset_t[i].id == id) {
			return this->preset.preset_t[i].type;
		}
	}
	if (id >= constants::offset_input_neuron_id && id < this->input_n + constants::offset_input_neuron_id) {
		return INPUT_NEURON;
	}
	if (id >= constants::offset_output_neuron_id && id < this->output_n + constants::offset_output_neuron_id) {
		return OUTPUT_NEURON;
	}
	if (id >= constants::offset_hidden_neuron_id && id < this->max_hidden_n + constants::offset_hidden_neuron_id) {
		return HIDDEN_NEURON;
	}
	if (id >= constants::offset_bias_neuron_id && id < this->bias_n + constants::offset_bias_neuron_id) {
		return BIAS_NEURON;
	}
	return HIDDEN_NEURON;
}

Configuration::Configuration()
{
	this->pool_quantity = 1;
	this->pool_population = 50;
	this->space_dimension = 3;

	this->input_n = 1;
	this->output_n = 1;
	this->bias_n = 1;

	this->max_hidden_n = 50;

	this->min_neuron_distance = 1;

	this->offset_input_neuron_id = 0;
	this->offset_bias_neuron_id = 10000;
	this->offset_hidden_neuron_id = 20000;
	this->offset_output_neuron_id = 30000;

	this->probabilities["mutate_link"] = 0.02; //mutate link's weight
	this->probabilities["mutate_bias"] = 0.02; //bias weight
	this->probabilities["mutate_node"] = 0.02; //add node
	this->probabilities["expressing"] = 0.02; // disable or enable a link
	this->probabilities["step"] = 0.05;
	this->probabilities["offset_range"] = 0.02;
	this->probabilities["w_purtubation"] = 0.98;//purtubation rate for weight

	this->probabilities["p_step"] = 1 / 3;
	this->probabilities["p_purtubation"] = 0.95;//purtubation rate for vector


	this->is_recurrent = true;

	this->initial_weight_range = 2;
	this->disjoint_weight = 1 / 3;
	this->disposition_weight = 1 / 3;
	this->dweight_weight = 1 / 3;
}

Configuration::Configuration(int input_n, int output_n)
{
	Configuration();
	this->input_n = input_n;
	this->output_n = output_n;
}

Configuration::~Configuration()
{
}

void Configuration::load_preset(const Preset& preset)
{
	this->preset = preset;
}
