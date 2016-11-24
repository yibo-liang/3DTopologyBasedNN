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

	vector<double> get_outputs(); //get current values of output nodes as a vector

	void step(vector<double> inputs);  //step one time frame

	int new_signal_id();

	Network(const Network& obj);

	Network();
	
	~Network();

private:
	int current_step;
	int signal_count;
};
vector<double> evaluate_network(
	Network netowrk, //the network to evaluate
	const  vector<vector<double>>& inputs, // vector of inputs vectors
	const  vector<vector<double>>& expect_outputs, //vector of expected outputs vectors
	int input_step_count, //how many time steps each input  vector is repeated 
	int output_step_count, //how many time steps each output vector is expected
	int output_time_offset);
#endif // !_NETWORK
