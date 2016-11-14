#pragma once

#ifndef PRESET_N
#define PRESET_N
#include <vector>
#include <map>
#include <iostream>
#include "LGene.h"
#include "TGene.h"
#include "globals.h"

using namespace constants;
using namespace std;
//struct preset_lgene {
//	int node_in;
//	int node_out;
//	double weight;
//	bool enabled;
//};
//
//struct preset_tgene {
//	int id;
//	int base_id;
//	double(*activation)(vector<double>);
//	int type;
//	vector<double>  offset;
//};

class Preset
{
public:

	vector<LGene> preset_l;
	map<int, TGene> preset_t;

	void add_t_gene(int id, int base, int neuron_type, double(*activation)(vector<double>), vector<double> offset);
	void add_l_gene(int node_in, int node_in_type, int node_out, int node_out_type, double weight);
	void add_l_gene(int node_in, int node_in_type, int node_out, int node_out_type, double weight, bool enabled);
	

	Preset();
	~Preset();

private:
};

#endif // !PRESET
