#pragma once

#ifndef EDGE
#define EDGE

#include "Signal.h"
#include <list>

using namespace std;

class Edge
{
public:

	int id;
	int node_in;
	int node_out;
	double length; //length as the multiplication of a single time step

	list<Signal> transmitting_signals;
	
	Edge(const Edge& obj);
	Edge();
	~Edge();

private:

};

#endif // !EDGE
