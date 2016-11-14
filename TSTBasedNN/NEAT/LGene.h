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

	LGene(const LGene &obj);

	LGene();
	~LGene();

private:

};


#endif // !GENE
