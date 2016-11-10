
#include "Configuration.h"


Configuration::Configuration()
{
	this->pool_quantity = 1;
	this->pool_population = 50;
	this->space_dimension = 3;

	this->input_n = 1;
	this->output_n = 1;

	this->max_neuron = 50;

	this->offset_hidden_neuron_id = 10000;
	this->offset_input_neuron_id = 0;
	this->offset_output_neuron_id = 20000;

	this->probabilities["link"] = 0.02;
	this->probabilities["bias"] = 0.02;
	this->probabilities["node"] = 0.02;
	this->probabilities["expressing"] = 0.02;
	this->probabilities["step"] = 0.02;
	this->probabilities["offset_range"] = 0.02;

	this->isBiased = true;

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