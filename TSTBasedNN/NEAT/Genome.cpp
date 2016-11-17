#include "../stdafx.h"

#include "Configuration.h"
#include "Genome.h"

#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <functional>
#include <ctime>
#include <iterator>
#include <string>

template <typename T>
//vector operator for addition
std::vector<T> operator+=(std::vector<T>& a, std::vector<T>& b)
{
	assert(a.size() == b.size());

	std::transform(a.begin(), a.end(), b.begin(),
		a.begin(), std::plus<T>());
	return a;
}

template <typename T>
std::vector<T> operator+(std::vector<T>& a, std::vector<T>& b)
{
	assert(a.size() == b.size());
	vector<T> c;
	std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(c),
		[](double a, double b) { return fabs(a + b); });
	return c;
}


double random() {
	unsigned int time_ui = unsigned int(time(NULL));
	srand(time_ui);
	return ((double)rand() / (RAND_MAX));
}

double Genome::disjointCompare(Genome g1, Genome g2)
{
	vector<LGene> genes1 = g1.l_genes;
	vector<LGene> genes2 = g2.l_genes;

	map<int, bool> li1;
	int c1 = 0, c2 = 0;
	for (auto it = genes1.begin(); it != genes1.end(); it++) {

		li1[it->innovation] = true;
		c1++;
	}

	map<int, bool> li2;
	for (auto it = genes2.begin(); it != genes2.end(); it++) {

		li2[it->innovation] = true;
		c2++;
	}

	int disjoint_count = 0;
	for (auto it = genes1.begin(); it != genes1.end(); it++) {
		if (!li2.count(it->innovation)) {
			disjoint_count++;
		}
	}
	for (auto it = genes2.begin(); it != genes2.end(); it++) {
		if (!li1.count(it->innovation)) {
			disjoint_count++;
		}
	}
	double n = fmax(c1, c2);
	if (n == 0) return 0;
	return (double)disjoint_count / n;

}

double Genome::weightCompare(Genome g1, Genome g2)
{
	vector<LGene> * genes1 = &g1.l_genes;
	vector<LGene> * genes2 = &g2.l_genes;
	map<int, LGene> i2;
	for (auto it = genes2->begin(); it != genes2->end(); it++) {
		i2[it->innovation] = *it;
	}
	double sum = 0;
	double coincident = 0;
	for (auto it = genes1->begin(); it != genes1->end(); it++) {
		LGene gene1 = *it;
		if (i2.count(it->innovation) > 0) {
			LGene gene2 = i2[it->innovation];
			sum = sum + abs(gene2.weight - gene1.weight);
			coincident++;
		}
	}
	if (sum == 0) return 0;
	return sum / coincident;
}

vector<double> randomVec(int dim) {
	vector<double> result;
	//random value -100 to 100 for each dimention of this vec
	for (int i = 0; i < dim; i++) {
		result.push_back(random() * 200 - 100);
	}
	return (result);
}

double distance(vector<double> v1, vector<double> v2) {
	//euclidean distance
	if (v1.size() == v2.size()) {
		double sum = 0;
		for (int i = 0; i < v1.size(); i++) {
			sum += pow(v1[i] - v2[i], 2);
		}
		return sqrt(sum);
	}
	else {
		throw exception("Vector does not match");
	}
}

double Genome::dispositionCompare(Genome g1, Genome g2)
{
	vector<TGene> * genes1 = &g1.t_genes;
	vector<TGene> * genes2 = &g2.t_genes;
	map<int, TGene> i2;
	for (vector<TGene>::iterator it = genes2->begin(); it != genes2->end(); it++) {
		i2[it->innovation] = *it;
	}
	double sum = 0;
	double coincident = 0;
	for (auto it = genes1->begin(); it != genes1->end(); it++) {
		TGene gene = *it;
		if (i2.count(gene.innovation) > 0) {
			TGene gene2 = i2[gene.innovation];
			sum += distance(gene.offset, gene2.offset);
			coincident++;
		}
	}

	if (sum == 0) return 0;
	return sum / coincident;
}

bool Genome::isSameSpecies(const Genome& another)
{
	double threshold = 1.0;
	double disjoint = this->disjointCompare(*this, another);
	double disposition = this->dispositionCompare(*this, another);
	double weight = weightCompare(*this, another);
	double sum = disjoint* this->configuration.disjoint_weight
		+ disposition* this->configuration.disposition_weight
		+ weight* this->configuration.dweight_weight;
	cout << "disjoint:" << disjoint << ", dipos:" << disposition << ", weight:" << weight << endl;
	return (sum < threshold);
}


Genome::Genome(const Genome & obj)
{
	//copy
	for (int i = 0; i < obj.t_genes.size(); i++) {
		this->t_genes.push_back(*new TGene(obj.t_genes[i]));
	}
	for (int i = 0; i < obj.l_genes.size(); i++) {
		this->l_genes.push_back(*new LGene(obj.l_genes[i]));
	}

	this->bias_n = obj.bias_n;
	this->hidden_n = obj.hidden_n;
	this->inno_func = obj.inno_func;
	this->reset_inno = obj.reset_inno;

}

Genome::Genome(const Configuration& configuration)
{
	this->configuration = configuration;

}

Genome::~Genome()
{
	this->network.~Network();
	for (int i = 0; i < this->t_genes.size(); i++) {
		delete &this->t_genes[i];
	}
	for (int i = 0; i < this->l_genes.size(); i++) {
		delete &this->l_genes[i];
	}
	this->configuration.~Configuration();
	delete &this->network;
	delete &this->configuration;

}


void Genome::init()

{
	/*
	this funciton should only be called once, when the pool is created and initial genomes are added

	add all tgene --- the position of the neurons, from the preset
	then iterate lgene, assign node that are not defined by tgene with random value,

	create all tgene by iterating lgene first
	firstly assign innovation to tgenes, node comes first
	if no preset lgene
	create tgene according to configuration: input_n; output_n; bias_n. Hidden neuron will be minimum at the beginning
	*/

	reset_inno();//reset innovation function for the initialisation of this genome
	int tgene_preset_n = this->configuration.preset.preset_t.size();
	int lgene_preset_n = this->configuration.preset.preset_l.size();
	this->hidden_n = 0;
	this->bias_n = 0;

	map<int, bool> node_created;
	//create tgene from preset tgene
	for (int i = 0; i < tgene_preset_n; i++) {
		TGene * tg = new TGene(configuration.preset.preset_t[i]);
		tg->innovation = inno_func();
		node_created[configuration.preset.preset_t[i].id] = true;

		if (tg->type == HIDDEN_NEURON) this->hidden_n++;
		if (tg->type == BIAS_NEURON) this->bias_n++;

		this->t_genes.push_back(*tg);
	}
	//create tgene from preset lgene
	for (int i = 0; i < lgene_preset_n; i++) {
		LGene * lg = new LGene(configuration.preset.preset_l[i]);

		//for node in of this lgene
		if (node_created.count(lg->node_in) == 0) {
			TGene * tg = new TGene();
			tg->id = lg->node_in;
			tg->enabled = true;
			tg->offset = randomVec(this->configuration.space_dimension);
			tg->innovation = inno_func();
			tg->type = this->configuration.get_node_type_from_id(tg->id);
			tg->base = -1;
			for (int j = 0; j < lgene_preset_n; j++) {
				if (configuration.preset.preset_l[j].node_out == tg->id) {
					tg->base = configuration.preset.preset_l[j].node_in;
				}
			}

			if (tg->type == HIDDEN_NEURON) this->hidden_n++;
			if (tg->type == BIAS_NEURON) this->bias_n++;

			this->t_genes.push_back(*tg);

			node_created[tg->id] = true;
		}
		else {

		}
		//for node out of this lgene
		if (node_created.count(lg->node_out) == 0) {
			TGene * tg = new TGene();
			tg->id = lg->node_out;
			tg->enabled = true;
			tg->offset = randomVec(this->configuration.space_dimension);
			tg->innovation = inno_func();
			tg->type = this->configuration.get_node_type_from_id(tg->id);
			tg->base = this->configuration.preset.preset_t[lg->node_in].id;

			if (tg->type == HIDDEN_NEURON) this->hidden_n++;
			if (tg->type == BIAS_NEURON) this->bias_n++;

			this->t_genes.push_back(*tg);
			node_created[tg->id] = true;
		}
		else {

		}

	}

	//create tgene from preset input, output, bias number; hidden neuron will not be created here since we evolve them

	int types[4] = { INPUT_NEURON, OUTPUT_NEURON, BIAS_NEURON };
	for (int t = 0; t < 4; t++) {
		int n;
		if (types[t] == INPUT_NEURON) n = this->configuration.input_n;
		else if (types[t] == OUTPUT_NEURON) n = this->configuration.output_n;
		else n = this->configuration.bias_n;


		for (int i = 0; i < n; i++) {
			int id = i + constants::get_id_offset(types[t]);
			if (node_created.count(id) == 0) {
				TGene * tg = new TGene();
				tg->id = id;
				tg->enabled = true;
				tg->offset = randomVec(this->configuration.space_dimension);
				tg->innovation = inno_func();
				tg->type = types[t];
				tg->base = -1;
				this->t_genes.push_back(*tg);
			}
		}
	}

	//also add bias_number
	this->bias_n = this->configuration.bias_n;
}

Network Genome::toNeuralNetwork()
{
	Network * network = new Network();

	//firstly express all tgenes with -1 base, that is, expressing all self position based neurons
	int expressed = 0;
	for (int i = 0; i < this->t_genes.size(); i++) {
		if (this->t_genes[i].base == -1) {
			Node * node = new Node();
			node->position = this->t_genes[i].offset;
			node->activation = this->t_genes[i].activation;
			node->type = this->t_genes[i].type;
			network->nodes[node->id] = *node;
			expressed++;
		}
	}
	//then add all node that is not expressed but of which base is expressed
	//should take O(nlogn) time

	while (expressed < this->t_genes.size()) {
		for (int i = 0; i < this->t_genes.size(); i++) {
			int base_id = this->t_genes[i].base;
			int id = this->t_genes[i].id;
			if (network->nodes.count(base_id) >0 && network->nodes.count(id) == 0) {
				Node * node = new Node();
				//node 's position is calculated by adding its offset on base's position
				node->position = this->t_genes[i].offset + network->nodes[base_id].position;
				node->activation = this->t_genes[i].activation;
				node->type = this->t_genes[i].type;
				network->nodes[node->id] = *node;
				expressed++;
			}
		}
	}
	//all tgene expressed, now lgenes

	for (int i = 0; i < this->l_genes.size(); i++) {
		Edge * edge = new Edge();
		edge->id = i;
		edge->node_in = this->l_genes[i].node_in;
		edge->node_out = this->l_genes[i].node_out;
		edge->length = distance(network->nodes[edge->node_in].position, network->nodes[edge->node_out].position);
		network->edges[edge->id] = *edge;
	}
	return *network;
}



//
//Network Genome::toNeuralNetwork()
//{
//	Network * network = new Network();
//
//	int dimension = this->configuration.space_dimension;
//
//	//input layer nodes
//	for (int i = 0; i < this->configuration.input_n; i++) {
//		Node * node = new Node();
//		//if preset config for the node
//		if (this->configuration.preset_input.count(i)) {
//			node->vec_offset = *new vector<double>(this->configuration.preset_input[i].offset);
//		}
//		node->id = i + this->configuration.offset_input_neuron_id;
//		node->type = "input";
//		network->nodes[node->id] = *node;
//
//	}
//	//hidden layer nodes
//	for (int i = 0; i < this->configuration.preset_hidden.size(); i++) {
//		Node * node = new Node();
//		//preset
//		if (this->configuration.preset_hidden.count(i)) {
//			node->vec_offset = *new vector<double>(this->configuration.preset_hidden[i].offset);
//		}
//		node->id = i + this->configuration.offset_hidden_neuron_id;
//		node->type = "hidden";
//		network->nodes[node->id] = *node;
//	}
//	//ouptut layer nodes
//	for (int i = 0; i < this->configuration.output_n; i++) {
//		Node * node = new Node();
//		if (this->configuration.preset_output.count(i)) {
//			node->vec_offset = *new vector<double>(this->configuration.preset_output[i].offset);
//		}
//		node->id = i + this->configuration.offset_output_neuron_id;
//		node->type = "output";
//		network->nodes[node->id] = *node;
//	}
//
//	//bias node , is treated as an input node
//	if (this->configuration.is_biased) {
//		Node * bias = new Node();
//		bias->id = this->configuration.input_n + 1;
//		bias->type = "bias";
//		network->nodes[bias->id] = *bias;
//	}
//
//	//creating nodes and edges according to genes
//			//create node according to each gene's OUT node
//	int edge_count = 0;
//	for (int i = 0; i < this->l_genes.size(); i++) {
//		LGene * gene = &this->l_genes[i];
//		if (gene->enabled) {
//			//create edge according to each gene
//			Edge* edge = new Edge();
//			network->edges[edge_count] = *edge;
//			int id = gene->node_out;
//			if (network->nodes.count(id) == 0) {
//				Node * node = new Node();
//				network->nodes[id] = *node;
//			}
//
//			network->nodes[id].edges_in.push_back(edge_count);
//			network->nodes[id].vec_offset += gene->offset_vector;
//
//			edge_count++;
//		}
//	}
//	//create node according to each gene's IN node
//	edge_count = 0;
//	for (int i = 0; i < this->l_genes.size(); i++) {
//		LGene * gene = &this->l_genes[i];
//		if (gene->enabled) {
//
//			if (network->nodes.count(gene->node_in) == 0) {
//				Node * node = new Node();
//				network->nodes[gene->node_in] = *node;
//			}
//			network->nodes[gene->node_out].edges_out.push_back(edge_count);
//			edge_count++;
//		}
//	}
//
//	return *network;
//
//}

int Genome::randomNeuron(bool inclInput, bool inclBias)
{
	srand(time(NULL));
	int count = t_genes.size();
	int select = rand() % count;
	return t_genes[select].id;
}

bool Genome::containsLGene(const LGene & lgene)
{
	for (int i = 0; i < this->l_genes.size(); i++) {
		if (this->l_genes[i].node_in = lgene.node_in &&  this->l_genes[i].node_out == lgene.node_out)
		{
			return true;
		}
	}
	return false;
}

TGene& Genome::get_tgene_by_id(int id)
{
	for (int i = 0; i < this->t_genes.size(); i++) {
		if (this->t_genes[i].id == id) {
			return this->t_genes[i];
		}
	}

	throw exception("No Tgene found with id");
}

vector<double> vecs_mid_point(const vector<double> &vec1, const vector<double> &vec2) {
	vector<double> result;
	for (int i = 0; i < vec1.size(); i++) {
		result.push_back((vec1[i] + vec2[i]) / 2);
	}
	return result;
}

vector<double> vec_in_sphere_between(const vector<double> &vec1, const vector<double> &vec2) {

	//diameter
	double d = distance(vec1, vec2);
	//centre
	vector<double> o = vecs_mid_point(vec1, vec2);
	vector<double> result;
	bool done = false;
	while (!done) {
		for (int i = 0; i < vec1.size(); i++) {
			double dv = abs(vec1[i] - vec2[i]);
			double v = min(vec1[i], vec2[i]) + random() *dv;
			result.push_back(dv);
		}
		if (distance(result, o) < d / 2) {
			done = true;
		}
	}
	return result;
}

//mutation that addes a new link(edge) between exisiting nodes
void add_link_mutate(Genome * g) {
	int n1 = g->randomNeuron(true, g->configuration.bias_n > 0);
	int n2 = g->randomNeuron(false, false);
	if (n1 == n2 && !g->configuration.is_recurrent) return;
	LGene * new_lgene = new LGene();
	new_lgene->node_in = n1;
	new_lgene->node_out = n2;
	if (g->containsLGene(*new_lgene)) {
		return;
	}


	new_lgene->weight = random()
		* g->configuration.initial_weight_range * 2
		- g->configuration.initial_weight_range;
	int innovation = g->inno_func();
	new_lgene->innovation = innovation;
	new_lgene->enabled = true;
	g->l_genes.push_back(*new_lgene);

}

// add a new hidden or bias node mutation
void add_node_mutate(Genome * g) {
	int gene_num = g->l_genes.size();
	if (gene_num == 0) {
		return;
	}
	int random_id = random() * gene_num;
	LGene * rand_gene = &g->l_genes[random_id];
	if (!rand_gene->enabled) {
		return;
	}

	bool isHidden = (random() < 0.9);
	//add tgene for the node, whose position 
	TGene * tg = new TGene();
	if (isHidden) {
		g->hidden_n++;
		tg->type = HIDDEN_NEURON;
	}
	else {
		g->bias_n++;
		tg->type = BIAS_NEURON;
	}
	rand_gene->enabled = false;
	TGene * out = &g->get_tgene_by_id(rand_gene->node_out);
	//copy offset and half both
	for (int i = 0; i < out->offset.size(); i++) {
		tg->offset.push_back(out->offset[i] / 2);
		out->offset[i] = out->offset[i] / 2;
	}


	LGene * gene1 = new LGene(*rand_gene);
	gene1->node_out = g->hidden_n;
	gene1->weight = 1;
	gene1->enabled = true;
	gene1->innovation = g->inno_func();
	LGene * gene2 = new LGene(*rand_gene);
	gene2->node_in = g->hidden_n;
	gene2->enabled = true;
	gene2->innovation = g->inno_func();

	g->l_genes.push_back(*gene1);
	g->l_genes.push_back(*gene2);
	g->t_genes.push_back(*tg);

}


//change weight of a random link 
void lpoint_mutate(Genome * g) {
	double step = g->configuration.probabilities["step"];

	for (int i = 0; i < g->l_genes.size(); i++) {
		LGene * gene = &g->l_genes[i];
		if (random() < g->configuration.probabilities["w_purtubation"]) {
			gene->weight = gene->weight + random()*step * 2 - step;
		}
		else {
			gene->weight = random() * 4 - 2;
		}
	}
}

void purtubate_vector(double step, double p_chance, vector<double> * vec) {
	for (int i = 0; i < vec->size(); i++) {
		if (random() < p_chance) {
			(*vec)[i] = (*vec)[i] + random()*step * 2 - step;
		}
		else {
			(*vec)[i] = random()*(*vec)[i] * 2 - (*vec)[i];
		}
	}
}

void tpoint_mutate(Genome * g) {
	double step = g->configuration.probabilities["p_step"];
	for (int i = 0; i < g->t_genes.size(); i++) {
		TGene * gene = &g->t_genes[i];
		double p_chance = g->configuration.probabilities["p_purtubation"];
		purtubate_vector(step, p_chance, &(gene->offset));
	}
}


vector<TGene*> get_tgene_not_based_on(Genome * g, TGene * tg) {
	vector<TGene*> result;
	vector<TGene>* tgs = &g->t_genes;
	map<int, bool> result_map;
	result.push_back(tg);
	if (tg->base == -1) return result;
	result_map[tg->id] = true;
	bool done = false;
	int k = result_map.size();
	while (!done) {
		for (int i = 0; i < g->t_genes.size(); i++) {
			TGene * ptr_tg = &g->t_genes[i];
			if (result_map.count(ptr_tg->id) == 0 && result_map.count(ptr_tg->base)>0) {
				result_map[ptr_tg->id] = true;
				result.push_back(ptr_tg);
			}
		}
		if (result_map.size() == k) {
			break;
		}
		else {
			k = result_map.size();
		}
	}
	return result;
}

void rebase_mutate(Genome * g) {
	int select = random()*g->t_genes.size();
	TGene * tg = &g->t_genes[select];
	if (!tg->fixed) { // if not a fixed node
		vector<TGene*> choices = get_tgene_not_based_on(g, tg);//choices of new base node
		int new_base = random()*choices.size();
		TGene * nb = choices[new_base];
		tg->base = nb->id;
		tg->innovation = g->inno_func();//assign new innovation number since the base changes.
	}
}

void switch_link_mutate(Genome * g, bool on) {
	vector<int> candidates;
	for (int i = 0; i < g->l_genes.size(); i++) {
		if (g->l_genes[i].enabled!=on) {
			candidates.push_back(i);
		}
	}
	int choice = candidates[random()* candidates.size()];
	g->l_genes[choice].enabled = on;
}



Genome fromMutate(const Genome& g, int(*inno_func)())
{
	double rate = g.configuration.probabilities.at("mutation");
	Genome * mutant = new Genome(g);
	if (random() < rate) {
		const map<string, double>* p = &g.configuration.probabilities;
		//string mutations[] = { "lpoint_mutate","tpoint_mutate","rebase_mutate", "add_node_mutate", "add_link_mutate","switch_link_mutate"};
		if (random() < p->at("lpoint_mutate")) {
			lpoint_mutate(mutant);
		}
		if (random() < p->at("tpoint_mutate")) {
			tpoint_mutate(mutant);
		}
		if (random() < p->at("rebase_mutate")) {
			rebase_mutate(mutant);
		}
		if (random() < p->at("add_node_mutate")) {
			add_node_mutate(mutant);
		}
		if (random() < p->at("add_link_mutate")) {
			add_link_mutate(mutant);
		}
		if (random() < p->at("off_switch_link_mutate")) {
			switch_link_mutate(mutant, false);
		}
		if (random() < p->at("on_switch_link_mutate")) {
			switch_link_mutate(mutant,true);
		}
	}
	return *mutant;

}
