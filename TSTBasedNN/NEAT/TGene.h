#pragma once

#ifndef TOPOLOGYGENE
#define TOPOLOGYGENE

#include <vector>
using namespace std;
class TGene
{
public:
	//id of the neuron which this gene express
	int id;
	//ID of base of the node, which this node is based on
	int base;
	int type;
	int innovation;
	bool enabled;
	vector<double> offset;
	//Activation function of this neuron, which could have been changed by user or evolution process
	//takes a vector of double input, returns a double
	double(*activation)(vector<double>);

	TGene(const TGene &obj);

	TGene();
	~TGene();

private:

};


#endif // !TOPOLOGYGENE
