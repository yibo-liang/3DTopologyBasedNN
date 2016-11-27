
#include "Configuration.h"
#include "Genome.h"
#include "globals.h"

#include "Innovation.h"
#include <iostream>
#include <vector>
#include <iterator>
#include <assert.h>
#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
using namespace std;
using namespace constants;
template <typename T>
//vector operator for addition
std::vector<T> operator+=(std::vector<T> a, std::vector<T> b)
{
	assert(a.size() == b.size());

	std::transform(a.begin(), a.end(), b.begin(),
		a.begin(), std::plus<T>());
	return a;
}

template <typename T>
std::vector<T> operator+(const std::vector<T> a, const std::vector<T> b)
{
	assert(a.size() == b.size());

	std::vector<T> result;
	result.reserve(a.size());

	std::transform(a.begin(), a.end(), b.begin(),
		std::back_inserter(result), std::plus<T>());
	return result;
}

int Genome::inno_func()
{
	return Innovation::instance()->getInnovation(this->pool_id);
}

int Genome::reset_inno()
{
	Innovation::instance()->resetInnovation(this->pool_id);
	return 0;
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
	double sum = 0;/*
	double coincident = 0;
	for (auto it = genes1->begin(); it != genes1->end(); it++) {
		LGene gene1 = *it;
		if (i2.count(it->innovation) > 0) {
			LGene gene2 = i2[it->innovation];
			sum = sum + abs(gene2.weight - gene1.weight);
			coincident++;
		}
	}*/
	if (sum == 0) return 0;

	double N = max(g1.l_genes.size(), g2.l_genes.size());
	return sum / N;
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
	vector<TGene> genes1 = g1.t_genes;
	vector<TGene> genes2 = g2.t_genes;
	map<int, TGene> i2;
	for (int i = 0; i < genes2.size(); i++) {
		i2[genes2[i].innovation] = genes2[i];
	}
	double sum = 0;/*
	double coincident = 0;
	for (int i = 0; i < genes1.size();i++) {
		TGene gene = genes1[i];
		if (i2.count(gene.innovation) > 0) {
			TGene gene2 = i2[gene.innovation];
			sum += distance(gene.offset, gene2.offset);
			coincident++;
		}
	}*/
	double N = max(g1.t_genes.size(), g2.t_genes.size());
	if (sum == 0) return 0;
	return sum / N;
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
		TGene tg(obj.t_genes[i]);
		this->t_genes.push_back(tg);
	}
	for (int i = 0; i < obj.l_genes.size(); i++) {
		LGene lg(obj.l_genes[i]);
		this->l_genes.push_back(lg);
	}
	this->pool_id = obj.pool_id;
	this->bias_n = obj.bias_n;
	this->hidden_n = obj.hidden_n;
	this->configuration = obj.configuration;
	this->fitness = obj.fitness;
	this->mutation_record = obj.mutation_record;
	this->pool_id = obj.pool_id;
	this->shared_fitness = obj.shared_fitness;

}

Genome::Genome(const Configuration& configuration)
{
	this->configuration = configuration;

}

Genome::~Genome()
{

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
	for (auto it = this->configuration.preset.preset_t.begin();
		it != this->configuration.preset.preset_t.end(); it++) {
		TGene tg(it->second);
		tg.innovation = inno_func();
		node_created[it->second.id] = true;

		if (tg.type == HIDDEN_NEURON) this->hidden_n++;
		if (tg.type == BIAS_NEURON) this->bias_n++;

		this->t_genes.push_back(tg);
	}
	//create tgene from preset lgene
	for (auto it = this->configuration.preset.preset_l.begin();
		it != this->configuration.preset.preset_l.end(); it++) {
		LGene lg(*it);

		//for node in of this lgene
		if (node_created.count(lg.node_in) == 0) {
			TGene tg;
			tg.id = lg.node_in;
			tg.enabled = true;
			tg.offset = randomVec(this->configuration.space_dimension);
			tg.innovation = inno_func();
			tg.type = this->configuration.get_node_type_from_id(tg.id);
			tg.base = -1;
			for (int j = 0; j < lgene_preset_n; j++) {
				if (configuration.preset.preset_l[j].node_out == tg.id) {
					tg.base = configuration.preset.preset_l[j].node_in;
				}
			}

			if (tg.type == HIDDEN_NEURON) this->hidden_n++;
			if (tg.type == BIAS_NEURON) this->bias_n++;

			this->t_genes.push_back(tg);

			node_created[tg.id] = true;
		}
		else {

		}
		//for node out of this lgene
		if (node_created.count(lg.node_out) == 0) {
			TGene tg;
			tg.id = lg.node_out;
			tg.enabled = true;
			tg.offset = randomVec(this->configuration.space_dimension);
			tg.innovation = inno_func();
			tg.type = this->configuration.get_node_type_from_id(tg.id);
			tg.base = this->configuration.preset.preset_t[lg.node_in].id;

			if (tg.type == HIDDEN_NEURON) this->hidden_n++;
			if (tg.type == BIAS_NEURON) this->bias_n++;

			this->t_genes.push_back(tg);
			node_created[tg.id] = true;
		}
		else {

		}

	}

	//create tgene from preset input, output, bias number; hidden neuron will not be created here since we evolve them

	int types[3] = { INPUT_NEURON, OUTPUT_NEURON, BIAS_NEURON };
	for (int t = 0; t < 3; t++) {
		int n;
		if (types[t] == INPUT_NEURON) n = this->configuration.input_n;
		else if (types[t] == OUTPUT_NEURON) n = this->configuration.output_n;
		else n = this->configuration.bias_n;


		for (int i = 0; i < n; i++) {
			int id = i + constants::get_id_offset(types[t]);
			if (node_created.count(id) == 0) {
				TGene tg;
				tg.id = id;
				tg.enabled = true;
				tg.offset = randomVec(this->configuration.space_dimension);
				tg.innovation = inno_func();
				tg.type = types[t];
				tg.base = -1;
				this->t_genes.push_back(tg);
			}
		}
	}

	//also add bias_number
	this->bias_n = this->configuration.bias_n;
	std::cout << "tgene:" << this->t_genes.size() << ", lgene:" << this->l_genes.size() << endl;
}

Network Genome::toNeuralNetwork()
{
	Network network;

	//firstly express all tgenes with -1 base, that is, expressing all self position based neurons
	int expressed = 0;
	for (int i = 0; i < this->t_genes.size(); i++) {
		if (this->t_genes[i].base == -1) {
			Node node;
			node.id = t_genes[i].id;
			node.position = this->t_genes[i].offset;
			if (node.position.size() == 0) {
				throw exception("");
			}
			node.type = this->t_genes[i].type;
			network.nodes[node.id] = node;
			expressed++;
		}
	}
	//then add all node that is not expressed but of which base is expressed
	//should take O(nlogn) time

	while (expressed < this->t_genes.size()) {
		for (int i = 0; i < this->t_genes.size(); i++) {
			int base_id = this->t_genes[i].base;
			int id = this->t_genes[i].id;
			if (network.nodes.count(base_id) > 0 && network.nodes.count(id) == 0) {
				Node node;
				//node 's position is calculated by adding its offset on base's position
				node.id = id;
				node.position = this->t_genes[i].offset + network.nodes[base_id].position;
				node.type = this->t_genes[i].type;
				network.nodes[node.id] = node;
				expressed++;
			}
		}
	}
	//all tgene expressed, now lgenes

	for (int i = 0; i < this->l_genes.size(); i++) {
		Edge edge;
		edge.id = i;
		edge.node_in = this->l_genes[i].node_in;
		edge.node_out = this->l_genes[i].node_out;
		edge.length = distance(network.nodes[edge.node_in].position, network.nodes[edge.node_out].position);
		//assign edge_in edge_out for connected node of this edge
		network.nodes[edge.node_in].edges_out.push_back(edge.id);
		network.nodes[edge.node_out].edges_in.push_back(edge.id);
		network.edges[edge.id] = edge;
	}


	network.configuration = Configuration(this->configuration);

	return network;
}



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
void add_link_mutate(Genome& g) {
	int n1 = g.randomNeuron(true, g.configuration.bias_n > 0);
	int n2 = g.randomNeuron(false, false);
	if (n1 == n2 && !g.configuration.is_recurrent) return;
	LGene new_lgene;
	new_lgene.node_in = n1;
	new_lgene.node_out = n2;
	if (g.containsLGene(new_lgene)) {
		return;
	}


	new_lgene.weight = random()
		* g.configuration.initial_weight_range * 2
		- g.configuration.initial_weight_range;
	int innovation = g.inno_func();
	new_lgene.innovation = innovation;
	new_lgene.enabled = true;
	g.l_genes.push_back(new_lgene);


	stringstream ss;
	ss << "add_link_mutate(" << n1 << " - " << n2 << ")" << endl;
	g.mutation_record += ss.str();

}

// add a new hidden or bias node mutation
void add_node_mutate(Genome& g) {
	int gene_num = g.l_genes.size();
	if (gene_num == 0) {
		return;
	}
	int random_id = random() * gene_num;
	LGene rand_gene = g.l_genes[random_id];

	stringstream ss;
	ss << "add_node_mutate(" << rand_gene.node_in << " <- " << rand_gene.node_out << ",";

	if (!rand_gene.enabled) {
		return;
	}

	bool isHidden = (random() < 0.9);
	//add tgene for the node, whose position 
	TGene  tg;
	if (isHidden) {
		tg.id = get_id_offset(HIDDEN_NEURON) + g.hidden_n;
		g.hidden_n++;
		tg.type = HIDDEN_NEURON;
	}
	else {
		tg.id = get_id_offset(BIAS_NEURON) + g.bias_n;
		g.bias_n++;
		tg.type = BIAS_NEURON;
	}
	tg.innovation = g.inno_func();
	rand_gene.enabled = false;
	TGene& out = g.get_tgene_by_id(rand_gene.node_out);
	//copy offset and half both
	for (int i = 0; i < out.offset.size(); i++) {
		tg.offset.push_back(out.offset[i] / 2);
		out.offset[i] = out.offset[i] / 2;
	}
	out.base = tg.id;
	//change the innovation of the node that is going to base on our new node
	out.innovation = g.inno_func();


	LGene gene1(rand_gene);
	gene1.node_out = tg.id;
	gene1.weight = 1;
	gene1.enabled = true;
	gene1.innovation = g.inno_func();
	LGene gene2(rand_gene);
	gene2.node_in = tg.id;
	gene2.enabled = true;
	gene2.innovation = g.inno_func();

	g.l_genes.push_back(gene1);
	g.l_genes.push_back(gene2);
	g.t_genes.push_back(tg);


	ss << " >> (" << gene1.node_in << " <- " << gene1.node_out << " <- " << gene2.node_out << ")" << endl;
	g.mutation_record += ss.str();

}


//change weight of a random link 
void lpoint_mutate(Genome& g) {
	double step = g.configuration.probabilities["step"];

	for (int i = 0; i < g.l_genes.size(); i++) {
		LGene * gene = &g.l_genes[i];
		if (random() < g.configuration.probabilities["w_purtubation"]) {
			gene->weight = gene->weight + random()*step * 2 - step;
		}
		else {
			gene->weight = random() * 4 - 2;
		}
	}
	stringstream ss;
	ss << "lpoint_mutate()" << endl;
	g.mutation_record += ss.str();
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

void tpoint_mutate(Genome& g) {
	double step = g.configuration.probabilities["p_step"];
	for (int i = 0; i < g.t_genes.size(); i++) {
		TGene * gene = &g.t_genes[i];
		double p_chance = g.configuration.probabilities["p_purtubation"];
		purtubate_vector(step, p_chance, &(gene->offset));
	}
	stringstream ss;
	ss << "tpoint_mutate()" << endl;
	g.mutation_record += ss.str();
}


vector<TGene> get_tgene_not_based_on(Genome& g, TGene& tg) {

	vector<TGene> tgs = g.t_genes;
	vector<TGene> result;
	map<int, bool> base_map;
	base_map[tg.id] = true;
	bool done = false;
	int k = base_map.size();
	while (!done) {
		for (int i = 0; i < g.t_genes.size(); i++) {
			TGene ptr_tg = g.t_genes[i];
			if (base_map.count(ptr_tg.id) == 0 && base_map.count(ptr_tg.base) > 0) {
				base_map[ptr_tg.id] = true;
				//tgenes_based_on_tg.push_back(ptr_tg);
			}
		}
		if (base_map.size() == k) {
			break;
		}
		else {
			k = base_map.size();
		}
	}
	for (int i = 0; i < g.t_genes.size(); i++) {
		TGene tg = g.t_genes[i];
		if (base_map.count(tg.id) == 0 && tg.enabled) {
			result.push_back(tg);
		}
	}
	return result;
}

void rebase_mutate(Genome& g) {
	int select = random()*g.t_genes.size();
	TGene tg = g.t_genes[select];

	stringstream ss;
	ss << "rebase(" << tg.base << "<-" << tg.id << ", ";

	if (!tg.fixed) { // if not a fixed node
		vector<TGene> choices = get_tgene_not_based_on(g, tg);//choices of new base node
		if (choices.size() == 0) return;
		int new_base = random()*choices.size();
		TGene nb = choices[new_base];
		tg.base = nb.id;
		tg.innovation = g.inno_func();//assign new innovation number since the base changes.
	}
	ss << tg.base << "<-" << tg.id << ")" << endl;
	g.mutation_record += ss.str();
}

void switch_link_mutate(Genome& g, bool on) {
	vector<int> candidates;
	for (int i = 0; i < g.l_genes.size(); i++) {
		if (g.l_genes[i].enabled != on) {
			candidates.push_back(i);
		}
	}
	if (candidates.size() == 0) return;
	int choice = candidates[random()* candidates.size()];
	g.l_genes[choice].enabled = on;

	stringstream ss;
	ss << "switch_link_mutate(" << choice << "," << on << "->" << !on << ")" << endl;
	g.mutation_record += ss.str();
}



Genome fromMutate(const Genome& g)
{
	double rate = g.configuration.probabilities.at("mutation");
	Genome mutant(g);
	if (random() < rate) {
		const map<string, double> p = g.configuration.probabilities;
		//string mutations[] = { "lpoint_mutate","tpoint_mutate","rebase_mutate", "add_node_mutate", "add_link_mutate","switch_link_mutate"};
		if (random() < p.at("lpoint_mutate")) {
			lpoint_mutate(mutant);
		}
		if (random() < p.at("tpoint_mutate")) {
			tpoint_mutate(mutant);
		}
		if (random() < p.at("rebase_mutate")) {
			rebase_mutate(mutant);
		}
		if (random() < p.at("add_node_mutate")) {
			add_node_mutate(mutant);
		}
		if (random() < p.at("add_link_mutate")) {
			add_link_mutate(mutant);
		}
		if (random() < p.at("off_switch_link_mutate")) {
			switch_link_mutate(mutant, false);
		}
		if (random() < p.at("on_switch_link_mutate")) {
			switch_link_mutate(mutant, true);
		}
	}
	return mutant;

}

Genome fromCrossOver(const Genome& g1, const Genome& g2)
{
	//map to analyse innovation of both genome
	map<int, TGene> inno_map_tgene;// innovation -> tgene
	map<int, vector<int>> id_inno_map; // id -> [innovation]

	//adding g1's tgene
	for (int i = 0; i < g1.t_genes.size(); i++) {
		inno_map_tgene[g1.t_genes[i].innovation] = TGene(g1.t_genes[i]);
		//record its innovation with id
		vector<int> tmp;
		tmp.push_back(g1.t_genes[i].innovation);
		id_inno_map[g1.t_genes[i].id] = tmp;
	}

	//adding g2's tgene, replace if it is better than g1's
	for (int i = 0; i < g2.t_genes.size(); i++) {
		TGene tg = g2.t_genes[i];
		int id = tg.id;
		int innovation = tg.innovation;
		if (inno_map_tgene.count(innovation) > 0) {
			if (g2.fitness > g1.fitness) {
				inno_map_tgene[innovation] = TGene(g2.t_genes[i]);
				id_inno_map[id].push_back(innovation);
			}
			else if (g2.fitness == g1.fitness && random() > 0.5) {
				//if somehow both genomes have the same fitness, such as 0, randomly select one
				inno_map_tgene[g2.t_genes[i].innovation] = TGene(g2.t_genes[i]);
				//same as above, add its innovation 
				id_inno_map[id].push_back(innovation);
			}
		}
		else {
			inno_map_tgene[innovation] = TGene(g2.t_genes[i]);
			id_inno_map[id].push_back(innovation);
		}
	}

	//get rid of the tgene that has duplicates with same id but different bases
	vector<TGene> result_tgene;
	for (auto it = id_inno_map.begin(); it != id_inno_map.end(); it++) {
		int id = it->first;
		vector<int> innovations = it->second;
		//choose randomly
		int select = random()*innovations.size();
		result_tgene.push_back(inno_map_tgene[innovations[select]]);
	}

	struct inno_compare {
		bool operator()(const TGene& tg1, const TGene& tg2) {
			return tg1.innovation < tg2.innovation;
		}
	};
	std::sort(result_tgene.begin(), result_tgene.end(), inno_compare());

	//lgenes cross over
	map<int, LGene> inno_map_lgene;
	for (int i = 0; i < g1.l_genes.size(); i++) {
		inno_map_lgene[g1.l_genes[i].innovation] = g1.l_genes[i];
	}
	for (int i = 0; i < g2.l_genes.size(); i++) {
		LGene lg2 = g2.l_genes[i];
		if (inno_map_lgene.count(lg2.innovation) > 0) {
			if (g2.fitness > g1.fitness) {
				inno_map_lgene[lg2.innovation] = LGene(lg2);
			}
			else if (g2.fitness == g1.fitness && random() > 0.5) {
				inno_map_lgene[lg2.innovation] = LGene(lg2);
			}
		}
		else {
			inno_map_lgene[lg2.innovation] = LGene(lg2);
		}
	}
	vector<LGene> result_lgene;
	for (auto it = inno_map_lgene.begin(); it != inno_map_lgene.end(); it++) {
		result_lgene.push_back(it->second);
	}


	Genome result_genome(g1);
	result_genome.t_genes = result_tgene;
	result_genome.l_genes = result_lgene;
	//recount bias_n and hidden_n
	int bias_n = 0;
	int hidden_n = 0;
	for (int i = 0; i < result_tgene.size(); i++) {
		if (result_tgene[i].type == HIDDEN_NEURON) {
			hidden_n++;
		}
		if (result_tgene[i].type == BIAS_NEURON) {
			bias_n++;
		}
	}
	result_genome.bias_n = bias_n;
	result_genome.hidden_n = hidden_n;



	return result_genome;
}
