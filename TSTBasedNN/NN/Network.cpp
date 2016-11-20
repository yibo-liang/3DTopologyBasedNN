
#include "../NEAT/globals.h"
#include "Network.h"

void Network::step(vector<double> inputs)
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

	//todo
	//firstly, activate all nodes, get the activation , insert into connected edges.
	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {

		if (!it->second.wake()) {
			continue;
		}

		double strength=it->second.activate(this->current_step);
		
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

}

int Network::new_signal_id()
{
	int result = this->signal_count;
	this->signal_count++;
	return result;
}

Network::Network(const Network & obj)
{
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

//this function given a network, and input/output values, then evaluate the network, gives an error rate 
vector<double> evaluate_network(
	Network netowrk, //the network to evaluate
	const  vector<vector<double>>& inputs, // vector of inputs vectors
	const  vector<vector<double>>& expect_outputs, //vector of expected outputs vectors
	int input_step_count, //how many time steps each input  vector is repeated 
	int output_step_count, //how many time steps each output vector is expected
	int output_time_offset) { //number of time stpes to ignore the output value



	vector<double> a;
	return a;
};