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

	vector<double> position;

	list<int> edges_in;
	list<int> edges_out;

	list<Signal> activation_signals;

	double(*activation)(vector<double>);

	Node();
	~Node();

private:

};

#endif // !NODE
