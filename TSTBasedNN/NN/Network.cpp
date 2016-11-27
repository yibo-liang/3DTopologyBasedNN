
#include "../NEAT/globals.h"
#include "Network.h"

bool Network::got_output()
{
	return _got_output;
}

void Network::notice_output()
{
	this->_got_output = true;
}

vector<double> Network::get_current_outputs_val()
{

	vector<double> result;
	for (int i = constants::offset_output_neuron_id; i < constants::offset_output_neuron_id + this->configuration.output_n; i++) {
		result.push_back(this->nodes[i].get_current_activation_value());
	}
	return vector<double>();
}

void Network::step()
{

	//firstly, activate all nodes, get the activation , insert into connected edges.
	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {

		if (!it->second.wake()) {
			continue;
		}

		double strength = it->second.activate(this->current_step);

		for (int i = 0; i < it->second.edges_out.size(); i++) {
			int edge_i = it->second.edges_out[i];
			Signal sig;
			sig.age = -1;//will be incremented by 1 to 0 in the following
			sig.id = this->new_signal_id();
			sig.strength = strength;
			this->edges[edge_i].transmitting_signals.push_back(sig);
		}
	}

	//all step edges
	for (auto it = this->edges.begin(); it != this->edges.end(); it++) {
		//for signals in each edge
		auto nit = it->second.transmitting_signals.begin();
		while (nit != it->second.transmitting_signals.end()) {
			nit->age++;
			//if the signal reach out of this edge, it is arriving the node .
			if (nit->age > it->second.length) {
				Signal sig(*nit);
				sig.age = sig.age - it->second.length;
				this->nodes[it->second.node_out].activation_signals.push_back(sig);//push to the next node
				nit = it->second.transmitting_signals.erase(nit);//remove this signal from current edge
			}
			else {
				++nit;
			}

		}

	}
	this->current_step++;
}

void Network::step_with_input(vector<double> inputs)
{
	//assign input value if any
	if (inputs.size() > 0) {
		int offset = constants::offset_input_neuron_id;
		int n = this->configuration.input_n;
		for (int i = 0; i < n; i++) {
			Signal nsig;
			nsig.age = 0;
			nsig.id = this->new_signal_id();
			nsig.strength = inputs[i];
			this->nodes[i].add_signal(nsig);
		}
	}
	this->step();


}

int Network::new_signal_id()
{
	int result = this->signal_count;
	this->signal_count++;
	return result;
}

Network::Network(const Network & obj)
{
	this->configuration = obj.configuration;
	this->current_step = obj.current_step;
	this->signal_count = obj.signal_count;
	for (auto it = this->edges.begin(); it != this->edges.end(); it++) {
		this->edges[it->first] = Edge(it->second);
	}
	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {
		this->nodes[it->first] = Node(it->second);
	}
}

Network::Network()
{
}

Network::~Network()
{

}


vector<vector<double>> get_network_result_on_each_node(Network& network, int output_length, int output_step_count) {

	vector<vector<double>> vec_outputs;
	int output_n = network.configuration.output_n;
	//for each output vector
	for (int j = 0; j < output_length; j++) {
		vector<double> output;

		//for each output node
		for (int i = 0; i < output_n; i++) {
			int id = i + constants::offset_output_neuron_id;
			Node& node = network.nodes[id];

			double val=0;
			int offset = j*output_step_count;
			//for every step_count results, calculate an average value as the value of that single output value
			for (int k = 0; k < output_step_count; k++) {
				if (node.activation_results.size() <= k + offset) {
					val += 0;
				}else
				val += node.activation_results[k + offset];
			}
			val = val / (double)output_step_count;
			output.push_back(val);
		}
		vec_outputs.push_back(output);
	}
	return vec_outputs;
}

bool wait_for_each_node(Network& network, int output_length, int output_step_count, int max_wait_step = 100) {
	bool done = false;
	int wait = 0;
	while (!done && wait < max_wait_step) {
		int output_n = network.configuration.output_n;
		done = true;
		for (int i = 0; i < output_n; i++) {
			int id = i + constants::offset_output_neuron_id;
			done = done && network.nodes[id].activation_results.size()>(output_length*output_step_count);
		}
		network.step();
		wait++;
	}
	return done;
}

//this function given a network, and input/output values, then evaluate the network, gives an error rate 
double evaluate_network_on_each_node(
	Network& network, //the network to evaluate
	vector<vector<double>>& inputs, // vector of inputs vectors
	vector<vector<double>>& expect_outputs, //vector of expected outputs vectors
	int input_step_count, //how many time steps each input  vector is repeated 
	int output_step_count //how many time steps each output vector is expected
	) {

	int input_length = inputs.size(); //quantity of input vectors

	for (int i = 0; i < input_length; i++) {
		for (int j = 0; j < input_step_count; j++) {
			//each input vector is repeatedly feed into network
			network.step_with_input(inputs[i]);
		}
	}

	//loop until each output nodes has enough output
	wait_for_each_node(network, expect_outputs.size(), output_step_count, 100);

	//when every nodes has enough valid activation result
	vector<vector<double>> outputs = get_network_result_on_each_node(network, expect_outputs.size(), output_step_count);

	//calculate mean square error
	double err = 0;
	for (int i = 0; i < expect_outputs.size(); i++) {
		for (int j = 0; j < network.configuration.output_n; j++) {
			double dx = (expect_outputs[i][j] - outputs[i][j]);
			err += dx*dx;
		}
	}
	err = err / (expect_outputs.size()* network.configuration.output_n);

	return err;
};