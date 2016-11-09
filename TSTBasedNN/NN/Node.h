#pragma once

#ifndef NODE
#define NODE

#include "Signal.h"
#include <list>
#include <vector>

using namespace std;

class Node
{
public:

	int id;
	
	string type;

	vector<double> vec_offset;

	list<int> edges_in;
	list<int> edges_out;

	list<Signal> activation_signals;

	Node();
	~Node();

private:

};

#endif // !NODE
