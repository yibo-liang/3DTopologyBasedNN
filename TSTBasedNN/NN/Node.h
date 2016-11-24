#pragma once

#ifndef NODE
#define NODE

#include "Signal.h"
#include "globals.h"
#include <vector>

using namespace std;

class Node
{
public:

	int id;
	
	int type;

	int activation_interval = constants::DEFAULT_ACTIVATION_INTERVAL;

	vector<double> position;

	vector<int> edges_in;
	vector<int> edges_out;

	vector<Signal> activation_signals;

	vector<double> activation_results;
	int activation_start=0;

	double(*activation)(vector<Signal>);

	double activate(int step, void(*notice)());
	bool wake();
	double get_current_activation_value();

	void add_signal(Signal sig);

	Node(const Node& obj);
	Node();
	~Node();

private:
	double current_val=0;
	int activated_step=0;
	int sleep_interval = 0;//will be incremented by min interval
};

double gaussian_activation(vector<Signal> sigs);

#endif // !NODE
