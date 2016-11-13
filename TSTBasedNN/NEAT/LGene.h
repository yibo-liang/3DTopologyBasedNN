#pragma once

#ifndef GENE
#define GENE

#include <vector>

using namespace std;

class LGene
{
public:


	//Typical NEAT genome
	int innovation;
	int node_in;
	int node_out;
	int enabled;
	double weight;


	//offset for node Time-Space Topology Based NN 
	//will add up 
	vector<double> offset_vector;
	
	LGene copy();

	LGene();
	~LGene();

private:

};


#endif // !GENE
