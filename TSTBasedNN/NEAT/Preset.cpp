
#include "Preset.h"


void Preset::add_t_gene(int id, int base, int neuron_type, double(*activation)(vector<double>), vector<double> offset)
{
	int id_offset = get_id_offset(neuron_type);
	TGene * tg = new TGene();
	tg->id = id+id_offset;
	tg->base = base;
	tg->activation = activation;
	for (int i = 0; i < offset.size(); i++) {
		tg->offset.push_back(offset[i]);
	}
		this->preset_t[id] = *tg;
}

void Preset::add_l_gene(int node_in, int node_in_type, int node_out, int node_out_type, double weight)
{
	add_l_gene( node_in,  node_in_type,  node_out,  node_out_type, true);
}



void Preset::add_l_gene(int node_in, int node_in_type, int node_out, int node_out_type, double weight, bool enabled)
{
	
	int in_offset = get_id_offset(node_in_type);
	int out_offset = get_id_offset(node_out_type);
	LGene * lg = new LGene();
	lg->node_in = node_in+in_offset;
	lg->node_out = node_out+out_offset;
	lg->weight = weight;
	lg->enabled = enabled;
	//innovation is not set yet, will be assigned in configuration
	this->preset_l.push_back(*lg);
}

Preset::Preset()
{

}

Preset::~Preset()
{
}

