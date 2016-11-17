#pragma once

#ifndef NODE
#define NODE

#include "Signal.h"
#include <vector>

using namespace std;

class Node
{
public:

	int id;
	
	string type;

	vector<double> position;

	vector<int> edges_in;
	vector<int> edges_out;

	vector<Signal> activation_signals;

	double(*activation)(vector<double>);
	Node(const Node& obj);
	Node();
	~Node();

private:

};

#endif // !NODE
