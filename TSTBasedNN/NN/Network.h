#pragma once

#ifndef _NETWORK
#define _NETWORK

#include <map>
#include <vector>
#include "Signal.h"
#include "Node.h"
#include "Edge.h"

using namespace std; 

class Network
{
public:

	map<int, Node> nodes;
	map<int, Edge> edges;

	vector<double> getOutputs();

	void step(vector<double> inputs);

	Network(const Network& obj);

	Network();
	
	~Network();

private:

};

#endif // !_NETWORK
