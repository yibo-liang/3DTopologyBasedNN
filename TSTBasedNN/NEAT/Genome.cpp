#include "../stdafx.h"

#include "Configuration.h"
#include "Genome.h"

#include <iostream>
#include <vector>

double Genome::disjointCompare(Genome g1, Genome g2)
{
	map<int, Gene> genes1 = g1.genes;
	map<int, Gene> genes2 = g2.genes;

	typedef map<int, Gene>::iterator map_iter;
	map<int, bool> li1;
	int c1 = 0, c2 = 0;
	for (map_iter it = genes1.begin(); it != genes1.end(); it++) {
		int key = it->first;
		Gene gene = it->second;
		li1[gene.innovation] = true;
		c1++;
	}

	map<int, bool> li2;
	for (map_iter it = genes2.begin(); it != genes2.end(); it++) {
		int key = it->first;
		Gene gene = it->second;
		li2[gene.innovation] = true;
		c2++;
	}

	int disjoint_count = 0;
	for (map_iter it = genes1.begin(); it != genes1.end(); it++) {
		int key = it->first;
		Gene gene = it->second;
		if (!li2.count(gene.innovation)) {
			disjoint_count++;
		}
	}
	for (map_iter it = genes2.begin(); it != genes2.end(); it++) {
		int key = it->first;
		Gene gene = it->second;
		if (!li1.count(gene.innovation)) {
			disjoint_count++;
		}
	}
	double n = fmax(c1, c2);
	if (n == 0) return 0;
	return (double)disjoint_count / n;

}

double Genome::weightCompare(Genome g1, Genome g2)
{
	map<int, Gene> genes1 = g1.genes;
	map<int, Gene> genes2 = g2.genes;
	typedef map<int, Gene>::iterator map_iter;
	map<int, Gene> i2;
	for (map_iter it = genes2.begin(); it != genes2.end(); it++) {
		int key = it->first;
		Gene gene = it->second;
		i2[gene.innovation] = gene;
	}
	double sum = 0;
	double coincident = 0;
	for (map_iter it = genes1.begin(); it != genes1.end(); it++) {
		int key = it->first;
		Gene gene = it->second;
		if (i2.count(gene.innovation) > 0) {
			Gene gene2 = i2[gene.innovation];
			sum = sum + abs(gene.weight - gene2.weight);
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
			sum += pow(v1[i]-v2[i], 2);
		}
		return sqrt(sum);
	}
	else {
		throw exception("Vector does not match");
	}
}

double Genome::dispositionCompare(Genome g1, Genome g2)
{
	map<int, Gene> genes1 = g1.genes;
	map<int, Gene> genes2 = g2.genes;
	typedef map<int, Gene>::iterator map_iter;
	map<int, Gene> i2;
	for (map_iter it = genes2.begin(); it != genes2.end(); it++) {
		int key = it->first;
		Gene gene = it->second;
		i2[gene.innovation] = gene;
	}
	double sum = 0;
	double coincident = 0;
	for (map_iter it = genes1.begin(); it != genes1.end(); it++) {
		int key = it->first;
		Gene gene = it->second;
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
	double sum =  disjoint+disposition +weight;
	cout << "disjoint:" << disjoint << ", dipos:" << disposition << ", weight:" << weight << endl;
	return (sum < threshold);
}

Genome::Genome()
{

}

Genome::~Genome()
{
}

Genome* basicGenome(Configuration config) {
	Genome* g= new Genome();

	g->input_n = config.input_n;
	g->output_n = config.output_n;
	g->max_neuron = config.max_neuron;
	return g;
}