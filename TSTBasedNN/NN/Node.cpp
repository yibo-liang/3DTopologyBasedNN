
#include <math.h>
#include "Node.h"
#include "globals.h"
#include "Signal.h"
#include "../NEAT/globals.h"

using namespace constants;


template <typename T>
T normal_pdf(T x, T m, T s)
{
	static const T inv_sqrt_2pi = 0.3989422804014327;
	T a = (x - m) / s;

	return inv_sqrt_2pi / s * std::exp(-T(0.5) * a * a);
}

//activation function that is
//tanh( sum (pdf(t_i)*x_i)/2)
//where t_i is the arriving time of i th signal and x_i is the strength of the i th
double gaussian_activation(vector<Signal> sigs)
{
	double result = 0;
	for (int i = 0; i < sigs.size(); i++) {
		result += normal_pdf<double>(sigs[i].age, 0, 0.5)*sigs[i].strength;

	}
	return tanh(result / 2);
}

double Node::activate(int step)
{

	double result = 0;
	//check if this node is already activated at T_i
	if (step < this->activated_step) {
		throw exception("Unexpected activate step");
	}
	if (this->sleep_interval <= 0) {
		this->sleep_interval += this->activation_interval;
	}
	else {
		return 0;
	}
	if (this->type == INPUT_NEURON) {
		if (this->activation_signals.size() > 0)
			result = this->activation_signals[0].strength;
		else
			result = 0;
		this->activation_signals.clear();
	}
	else if (this->type == BIAS_NEURON) {
		result = 1;
	}
	else if (this->type == OUTPUT_NEURON || this->type == HIDDEN_NEURON) {
		result = this->activation(this->activation_signals);
		for (int i = 0; i < this->activation_signals.size(); i++) {
			activation_signals[i].age++;
		}
	}

	//remove signals that are recevied long time ago by this node
	for (auto it = this->activation_signals.begin(); it != this->activation_signals.end();) {
		if (it->age > MAX_NEURON_ACTIVATION_SIGNAL_TIME) {
			it = this->activation_signals.erase(it);
		}
		else {
			++it;
		}
	}

	this->current_val = result;

	//save the result;
	if (this->type == OUTPUT_NEURON) {
		if (activation_start == 0) {
			activation_start = activated_step;
		}
		activation_results.push_back(result);
	}
	this->activated_step++;
	return result;
}

bool Node::wake()
{

	this->sleep_interval--;
	if (this->sleep_interval <= 0) {
		this->sleep_interval = 0;

		return true;
	}
	return false;
}

double Node::get_current_activation_value()
{
	return this->current_val;
}

void Node::add_signal(Signal sig)
{
	if (this->type == INPUT_NEURON) {
		if (this->activation_signals.size() > 0) {
			this->activation_signals[0] = (sig);
		}
	}
	else if (this->type == BIAS_NEURON) {
		return;
	}
	else {
		this->activation_signals.push_back(sig);
	}
}



Node::Node(const Node & obj)
{
	this->activation = obj.activation;
	this->edges_in = obj.edges_in;
	this->edges_out = obj.edges_in;
	this->activation_signals = obj.activation_signals;
	this->id = obj.id;
	this->position = obj.position;
	this->type = obj.type;
}

Node::Node()
{
	this->type = constants::HIDDEN_NEURON;
	this->activation = gaussian_activation;
}

Node::~Node()
{
}
