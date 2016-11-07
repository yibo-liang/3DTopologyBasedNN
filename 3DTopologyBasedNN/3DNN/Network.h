#pragma once

#ifndef _NETWORK
#define _NETWORK

#include <map>
#include "Signal.h"
#include "Node.h"
#include "Edge.h"

using namespace std; 

class Network
{
public:

	map<int, Node> nodes;
	map<int, Edge> edges;




	Network();
	~Network();

private:

};

#endif // !_NETWORK
