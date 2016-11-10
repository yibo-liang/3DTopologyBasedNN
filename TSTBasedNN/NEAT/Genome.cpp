#include "../stdafx.h"

#include "Configuration.h"
#include "Genome.h"

#include <iostream>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <functional>

template <typename T>
//vector operator for addition
std::vector<T> operator+=(std::vector<T>& a, std::vector<T>& b)
{
	assert(a.size() == b.size());

	std::transform(a.begin(), a.end(), b.begin(),
		a.begin(), std::plus<T>());
	return a;
}

typedef vector<Gene>::iterator map_iter;
double Genome::disjointCompare(Genome g1, Genome g2)
{
	vector<Gene> genes1 = g1.genes;
	vector<Gene> genes2 = g2.genes;

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
	vector<Gene> * genes1 = &g1.genes;
	vector<Gene> * genes2 = &g2.genes;
	map<int, Gene> i2;
	for (map_iter it = genes2->begin(); it != genes2->end(); it++) {
		i2[it->innovation] = *it;
	}
	double sum = 0;
	double coincident = 0;
	for (map_iter it = genes1->begin(); it != genes1->end(); it++) {

		if (i2.count(it->innovation) > 0) {
			Gene gene2 = i2[it->innovation];
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
	vector<Gene> * genes1 = &g1.genes;
	vector<Gene> * genes2 = &g2.genes;
	map<int, Gene> i2;
	for (map_iter it = genes2->begin(); it != genes2->end(); it++) {
		i2[it->innovation] = *it;
	}
	double sum = 0;
	double coincident = 0;
	for (map_iter it = genes1->begin(); it != genes1->end(); it++) {
		Gene gene = *it;
		if (i2.count(gene.innovation) > 0) {
			Gene gene2 = i2[gene.innovation];
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
}

Genome::~Genome()
{
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
	if (this->configuration.isBiased) {
		Node * bias = new Node();
		bias->id = this->configuration.input_n + 1;
		bias->type = "bias";
		network->nodes[bias->id] = *bias;
	}

	//creating nodes and edges according to genes
			//create node according to each gene's OUT node
	int edge_count = 0;
	for (int i = 0; i < this->genes.size(); i++) {
		Gene * gene = &this->genes[i];
		if (gene->enabled) {
			//create edge according to each gene
			Edge* edge = new Edge();
			network->edges[edge_count] = *edge;

			if (network->nodes.count(gene->node_out)==0) {
				Node * node = new Node();
				network->nodes[gene->node_out] = *node;
			}

			network->nodes[gene->node_out].edges_in.push_back(edge_count);
			network->nodes[gene->node_out].vec_offset += gene->offset_vector;
			
			edge_count++;
		}
	}
	//create node according to each gene's IN node
    edge_count = 0;
	for (int i = 0; i < this->genes.size(); i++) {
		Gene * gene = &this->genes[i];
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

Genome* basicGenome(Configuration config) {
	Genome* g = new Genome(config);

	g->input_n = config.input_n;
	g->output_n = config.output_n;
	g->max_neuron = config.max_neuron;
	return g;
}

Genome fromMutate(Genome g, map<string, double> probabilities, int(*inno_func)(int))
{
	
	return g;
}
