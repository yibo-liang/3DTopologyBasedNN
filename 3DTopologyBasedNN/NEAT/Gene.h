#pragma once

#ifndef GENE
#define GENE

class Gene
{
public:


	//Typical NEAT genome
	int innovation;
	int node_in;
	int node_out;
	double weight;


	//offset for 3D Topology NN 
	double offset_x, offset_y, offset_z; //offset of position in 3 dimensional space for out node,

	Gene copy();

	Gene();
	~Gene();

private:

};


#endif // !GENE
