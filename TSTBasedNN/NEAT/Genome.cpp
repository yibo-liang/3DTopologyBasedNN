#include "../stdafx.h"

#include "Configuration.h"
#include "Genome.h"

#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <functional>
#include <ctime>

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
std::vector<T> operator-(std::vector<T>& a, std::vector<T>& b)
{
	assert(a.size() == b.size());
	vector<T>& c;
	std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(c),
		[](double a, double b) { return fabs(a - b); });
	return c;
}


double random() {
	return ((double)rand() / (RAND_MAX));
}

typedef vector<LGene>::iterator map_iter;
double Genome::disjointCompare(Genome g1, Genome g2)
{
	vector<LGene> genes1 = g1.l_genes;
	vector<LGene> genes2 = g2.l_genes;

	map<int, bool> li1;
	int c1 = 0, c2 = 0;
	for (map_iter it = genes1.begin(); it != genes1.end(); it++) {

		li1[it->innovation] = true;
		c1++;
	}

	map<int, bool> li2;
	for (map_iter it = genes2.begin(); it != genes2.end(); it++) {

		li2[it->innovation] = true;
		c2++;
	}

	int disjoint_count = 0;
	for (map_iter it = genes1.begin(); it != genes1.end(); it++) {
		if (!li2.count(it->innovation)) {
			disjoint_count++;
		}
	}
	for (map_iter it = genes2.begin(); it != genes2.end(); it++) {
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
	for (map_iter it = genes2->begin(); it != genes2->end(); it++) {
		i2[it->innovation] = *it;
	}
	double sum = 0;
	double coincident = 0;
	for (map_iter it = genes1->begin(); it != genes1->end(); it++) {

		if (i2.count(it->innovation) > 0) {
			LGene gene2 = i2[it->innovation];
			sum = sum + abs(it->weight - it->weight);
			coincident++;
		}
	}
	if (sum == 0) return 0;
	return sum / coincident;
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
	vector<LGene> * genes1 = &g1.l_genes;
	vector<LGene> * genes2 = &g2.l_genes;
	map<int, LGene> i2;
	for (map_iter it = genes2->begin(); it != genes2->end(); it++) {
		i2[it->innovation] = *it;
	}
	double sum = 0;
	double coincident = 0;
	for (map_iter it = genes1->begin(); it != genes1->end(); it++) {
		LGene gene = *it;
		if (i2.count(gene.innovation) > 0) {
			LGene gene2 = i2[gene.innovation];
			sum = distance(gene.offset_vector, gene2.offset_vector);
			coincident++;
		}
	}

	if (sum == 0) return 0;
	return sum / coincident;
}

bool Genome::isSameSpecies(Genome another)
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


Genome::Genome(Configuration configuration)
{
	this->configuration = configuration;

	//add all initial neuron(node) id for further usage
	for (int i = 0; i < configuration.input_n; i++) {
		this->neurons[i + configuration.offset_input_neuron_id] = INPUT;
	}
	for (int i = 0; i < configuration.output_n; i++) {
		this->neurons[i + configuration.offset_output_neuron_id] = OUTPUT;
	}

	for (auto it = this->configuration.preset_hidden.begin();
	it != this->configuration.preset_hidden.end(); it++) {
		int i = it->second.id;
		this->neurons[i + configuration.offset_hidden_neuron_id] = HIDDEN;
	}
	this->hidden_n = configuration.preset_hidden.size();

}

Genome::~Genome()
{
}


Genome Genome::copy()
{
	Genome * copy = new Genome(this->configuration);
	for (int i = 0; i < this->l_genes.size(); i++) {
		copy->l_genes.push_back(this->l_genes[i].copy());
	}
	return *copy;


}

Network Genome::toNeuralNetwork()
{
	Network * network = new Network();

	int dimension = this->configuration.space_dimension;

	//input layer nodes
	for (int i = 0; i < this->configuration.input_n; i++) {
		Node * node = new Node();
		//if preset config for the node
		if (this->configuration.preset_input.count(i)) {
			node->vec_offset = *new vector<double>(this->configuration.preset_input[i].offset);
		}
		node->id = i + this->configuration.offset_input_neuron_id;
		node->type = "input";
		network->nodes[node->id] = *node;

	}
	//hidden layer nodes
	for (int i = 0; i < this->configuration.preset_hidden.size(); i++) {
		Node * node = new Node();
		//preset
		if (this->configuration.preset_hidden.count(i)) {
			node->vec_offset = *new vector<double>(this->configuration.preset_hidden[i].offset);
		}
		node->id = i + this->configuration.offset_hidden_neuron_id;
		node->type = "hidden";
		network->nodes[node->id] = *node;
	}
	//ouptut layer nodes
	for (int i = 0; i < this->configuration.output_n; i++) {
		Node * node = new Node();
		if (this->configuration.preset_output.count(i)) {
			node->vec_offset = *new vector<double>(this->configuration.preset_output[i].offset);
		}
		node->id = i + this->configuration.offset_output_neuron_id;
		node->type = "output";
		network->nodes[node->id] = *node;
	}

	//bias node , is treated as an input node
	if (this->configuration.is_biased) {
		Node * bias = new Node();
		bias->id = this->configuration.input_n + 1;
		bias->type = "bias";
		network->nodes[bias->id] = *bias;
	}

	//creating nodes and edges according to genes
			//create node according to each gene's OUT node
	int edge_count = 0;
	for (int i = 0; i < this->l_genes.size(); i++) {
		LGene * gene = &this->l_genes[i];
		if (gene->enabled) {
			//create edge according to each gene
			Edge* edge = new Edge();
			network->edges[edge_count] = *edge;
			int id = gene->node_out;
			if (network->nodes.count(id) == 0) {
				Node * node = new Node();
				network->nodes[id] = *node;
			}

			network->nodes[id].edges_in.push_back(edge_count);
			network->nodes[id].vec_offset += gene->offset_vector;

			edge_count++;
		}
	}
	//create node according to each gene's IN node
	edge_count = 0;
	for (int i = 0; i < this->l_genes.size(); i++) {
		LGene * gene = &this->l_genes[i];
		if (gene->enabled) {

			if (network->nodes.count(gene->node_in) == 0) {
				Node * node = new Node();
				network->nodes[gene->node_in] = *node;
			}
			network->nodes[gene->node_out].edges_out.push_back(edge_count);
			edge_count++;
		}
	}

	return *network;

}

int Genome::randomNeuron(bool inclInput, bool inclBias)
{
	srand(time(NULL));
	vector<int> candidates;
	typedef map<int, int>::iterator iter;
	for (iter it = this->neurons.begin(); it != this->neurons.end(); it++) {
		if (inclInput && it->second == INPUT
			|| inclBias && it->second == BIAS) {

		}
		else {
			candidates.push_back(it->first);
		}
	}
	int count = candidates.size();
	int select = rand() % count;
	return candidates[select];
}

Genome* basicGenome(Configuration config) {
	Genome* g = new Genome(config);

	return g;
}


vector<double> vec_in_sphere_between(vector<double> vec1, vector<double> vec2) {

	//diameter
	double d = distance(vec1, vec2);
	vector<double> * result = new vector<double>();
	for (int i = 0; i < vec1.size(); i++) {
		double dv = abs(vec1[i] - vec2[i]);
		double v = min(vec1[i], vec2[i]) + random() *dv;
		result->push_back(dv);
	}
	return *result;
}

//mutation that addes a new link(edge)
void link_mutate(Genome * g, int(*inno_func)()) {
	int n1 = g->randomNeuron(true, g->configuration.is_biased);
	int n2 = g->randomNeuron(false, false);
	LGene * new_gene = new LGene();
	new_gene->node_in = n1;
	new_gene->node_out = n2;
	if (g->containsLGene(*new_gene)) {
		return;
	}

	new_gene->weight = random()
		* g->configuration.initial_weight_range * 2
		- g->configuration.initial_weight_range;
	int innovation = inno_func();
	new_gene->innovation = innovation;

	//offset vector for new link,
	vector<double> vec1 = g->network.nodes[n1].vec_offset;
	vector<double> vec2 = g->network.nodes[n2].vec_offset;


	new_gene->offset_vector = vec_in_sphere_between(vec1, vec2);
	g->l_genes.push_back(*new_gene);

}

// add a new node mutation
void node_mutate(Genome * g, int(*inno_func)()) {
	int gene_num = g->l_genes.size();
	if (gene_num == 0) {
		return;
	}
	int random_id = random() * gene_num;
	LGene * rand_gene = &g->l_genes[random_id];
	if (!rand_gene->enabled) {
		return;
	}
	g->hidden_n++;
	rand_gene->enabled = false;
	LGene * gene1 = &(*rand_gene).copy();
	gene1->node_out = g->hidden_n;
	gene1->weight = 1;
	gene1->enabled = true;
	gene1->innovation = inno_func();
	LGene * gene2 = &(*rand_gene).copy();
	gene2->node_in = g->hidden_n;
	gene2->enabled = true;
	gene2->innovation = inno_func();

	g->l_genes.push_back(*gene1);
	g->l_genes.push_back(*gene2);

}


//change weight of a random link 
void point_mutate(Genome * g) {
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

void position_mutate(Genome * g) {
	double step = g->configuration.probabilities["p_step"];
	for (int i = 0; i < g->l_genes.size(); i++) {
		LGene * gene = &g->l_genes[i];
		for (int j = 0; j < gene->offset_vector.size(); j++) {
			if (random() < g->configuration.probabilities["p_purtubation"]) {
				gene->offset_vector[j] = gene->offset_vector[j] + random()*step * 2 - step;
			}
			else {

			}
		}
	}
}

Genome fromMutate(Genome g, int(*inno_func)())
{
	double rate = g.configuration.probabilities["mutation"];

	Genome * mutant = &g.copy();

}
