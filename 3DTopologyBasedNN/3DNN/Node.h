#pragma once

#ifndef NODE
#define NODE

#include "Signal.h"
#include <list>

using namespace std;

class Node
{
public:

	int id;
	
	double x, y, z;

	list<int> edges_in;
	list<int> edges_out;

	list<Signal> activation_signals;

	Node();
	~Node();

private:

};

#endif // !NODE
