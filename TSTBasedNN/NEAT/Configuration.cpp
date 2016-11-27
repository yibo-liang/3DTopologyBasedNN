
#include "Configuration.h"
#include "globals.h"

using namespace constants;
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
	
	this->probabilities["crossover"] = 1;
	this->probabilities["mutation"] = 1;
	
	this->probabilities["lpoint_mutate"] = 1; //mutate link's weight
	this->probabilities["tpoint_mutate"] = 1; //bias weight
	this->probabilities["add_node_mutate"] = 1; //add node
	this->probabilities["add_link_mutate"] = 1; //add node
	this->probabilities["rebase_mutate"] = 1; //add node
	this->probabilities["off_switch_link_mutate"] = 1; // disable or enable a link
	this->probabilities["on_switch_link_mutate"] = 1; // disable or enable a link

	this->probabilities["step"] = 0.05;
	//this->probabilities["offset_range"] = 0.02;
	this->probabilities["w_purtubation"] = 0.98;//purtubation rate for weight

	this->probabilities["p_step"] = 1 / 3;
	this->probabilities["p_purtubation"] = 0.95;//purtubation rate for vector


	this->is_recurrent = true;

	this->initial_weight_range = 2;
	this->disjoint_weight = 1 / 3;
	this->disposition_weight = 1 / 3;
	this->dweight_weight = 1 / 3;
}

Configuration::Configuration(const Configuration & obj)
{
	this->bias_n = obj.bias_n;
	this->disjoint_weight = obj.disjoint_weight;
	this->disposition_weight = obj.disposition_weight;
	this->dweight_weight = obj.dweight_weight;
	this->initial_weight_range = obj.initial_weight_range;
	this->input_n = obj.input_n;
	this->is_recurrent = obj.is_recurrent;
	this->max_hidden_n = obj.max_hidden_n;
	this->min_neuron_distance = obj.min_neuron_distance;
	this->offset_bias_neuron_id = obj.offset_bias_neuron_id;
	this->offset_hidden_neuron_id = obj.offset_hidden_neuron_id;
	this->offset_output_neuron_id = obj.offset_output_neuron_id;
	this->offset_input_neuron_id = obj.offset_input_neuron_id;
	this->output_n = obj.output_n;
	this->pool_population = obj.pool_population;
	this->pool_quantity = obj.pool_quantity;
	this->preset = Preset(obj.preset);
	this->probabilities = obj.probabilities;
	this->space_dimension = obj.space_dimension;
	this->is_elitism = obj.is_elitism;
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
