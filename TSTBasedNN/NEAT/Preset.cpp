
#include "Preset.h"
#include "globals.h"

void Preset::add_t_gene(int id, int base, int neuron_type, vector<double> offset)
{
	int id_offset = constants::get_id_offset(neuron_type);
	TGene tg;
	tg.id = id + id_offset;
	tg.base = base;
	tg.type = neuron_type;
	for (int i = 0; i < offset.size(); i++) {
		tg.offset.push_back(offset[i]);
	}
	this->preset_t[id+id_offset] = tg;
}

void Preset::add_l_gene(int node_in, int node_in_type, int node_out, int node_out_type, double weight)
{
	add_l_gene(node_in, node_in_type, node_out, node_out_type,weight, true);
}



void Preset::add_l_gene(int node_in, int node_in_type, int node_out, int node_out_type, double weight, bool enabled)
{

	int in_offset = constants::get_id_offset(node_in_type);
	int out_offset = constants::get_id_offset(node_out_type);
	LGene lg;
	lg.node_in = node_in + in_offset;
	lg.node_out = node_out + out_offset;
	lg.weight = weight;
	lg.enabled = enabled;
	//innovation is not set yet, will be assigned in configuration
	this->preset_l.push_back(lg);
}

Preset::Preset(const Preset & obj)
{
	this->preset_l = obj.preset_l;
	this->preset_t = obj.preset_t;
}

Preset::Preset()
{

}

Preset::~Preset()
{
	/*for (int i = 0; i < this->preset_l.size(); i++) {
		delete &this->preset_l[i];
	}
	for (auto it = this->preset_t.begin(); it != this->preset_t.end(); it++) {
		delete &it->second;
	}*/
}


