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
	int type; //input, output, hidden, bias
	int innovation;
	bool enabled;//if this node is good to be connected with other node
	bool fixed;//if this node is fixed in space, if not, position will be changed during evolutionary process
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
