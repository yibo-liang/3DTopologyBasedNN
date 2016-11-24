#pragma once

#ifndef _NETWORK
#define _NETWORK

#include <map>
#include <vector>
#include "Signal.h"
#include "Node.h"
#include "Edge.h"
#include "../NEAT/Configuration.h"

using namespace std;

class Network
{
public:
	Configuration configuration;

	map<int, Node> nodes;
	map<int, Edge> edges;

	
	bool got_output();
	void notice_output();

	vector<double> get_current_outputs_val(); //get current values of output nodes as a vector

	void step();
	void step_with_input(vector<double> inputs);  //step one time frame


	int new_signal_id();

	Network(const Network& obj);

	Network();

	~Network();

private:
	int current_step;
	int signal_count;
	bool _got_output=false;
};
double evaluate_network_on_each_node(
	Network& netowrk, //the network to evaluate
	vector<vector<double>>& inputs, // vector of inputs vectors
	vector<vector<double>>& expect_outputs, //vector of expected outputs vectors
	int input_step_count, //how many time steps each input  vector is repeated 
	int output_step_count); //how many time steps each output vector is expected);
#endif // !_NETWORK
