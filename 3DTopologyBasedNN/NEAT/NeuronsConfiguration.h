#pragma once

#ifndef NCONFIG
#define NCONFIG

#include <string>
using namespace std;

class NeuronsConfiguration
{
public:
	
	int count();
	string type;
	void addNeuron(NeuronInfo info);
	
	NeuronsConfiguration();
	~NeuronsConfiguration();

private:

};

class NeuronInfo {
public:
	double offset_x, offset_y, offset_z;

	NeuronInfo(double x, double y, double z);
	~NeuronInfo();
};


#endif