
#include "Pool.h"
#include "globals.h"
#include <iostream>

using namespace constants;
int Pool::resetInnovation()
{
	this->innovation = 0;
	return 0;
}

int Pool::getInnovation()
{
	int i = this->innovation;
	this->innovation++;
	return i;
}

void Pool::newGeneration()
{
	//for each species
	//for each genome, create a network
	//for each network, eval it.
	//eval(func, fcun){ }
	//selection + crossover
	//mutation
	//eliticism
	for (int sp_i = 0; sp_i < species.size(); sp_i++) {
		Species& sp = species[sp_i];
		for (int g_i = 0; g_i < sp.genomes.size(); g_i++) {
			Genome& g = sp.genomes[g_i];
			g.network = g.toNeuralNetwork;
			g.fitness=evaluate_network_on_each_node(g.network, inputs, outputs, DEFAULT_INPUT_STEP, DEFAULT_OUTPUT_STEP);
			g.shared_fitness = g.fitness / (double)sp.genomes.size();
		}
	}

	

	for (int sp_i = 0; sp_i < species.size(); sp_i++) {
		Species& sp = species[sp_i];
		Genome * g1; 
		Genome * g2;
		//if adoption
		if (random() < this->adoption_rate) {
			int alien_si =(int)( random()*species.size());
			int alien_gi = (int)(random()*species[alien_si].genomes.size());
			g1 = &(species[alien_si].genomes[alien_gi]);
		}
		else {
			int gi = (int)(random()*sp.genomes.size());
			g1 = &sp.genomes[gi];
		}
		int gi = (int)(random()*sp.genomes.size());
		g2 = &sp.genomes[gi];
		

	}


}

Pool::Pool(const Pool & obj)
{
	this->species = obj.species;
	this->outputs = obj.outputs;
	this->inputs = obj.inputs;
	this->innovation = obj.innovation;
	this->GASelection = obj.GASelection;
	this->generation = obj.generation;
	this->configuration = obj.configuration;
}

Pool::Pool()
{
	this->innovation = 1;
	this->generation = 0;
}

Pool::~Pool()
{
}

int Pool::genome_belong_to(Genome & genome)
{

	for (int si = 0; si < species.size();si++) {
		//std::cout << si._Ptr <<","<< si->genomes.size() << std::endl;
		int random_id = (int)(random()* species.size());
		Genome& president = species[si].genomes.at(random_id);
		if (genome.isSameSpecies(president)) {
			return si;
		}

	}
	return -1;
}



void Pool::addToSpecies(Genome& g)
{
	int sp_i = genome_belong_to(g);
	
	if (sp_i==-1) {
		Species new_species;
		g.inno_func = this->getInnovation;
		new_species.genomes.push_back(g);
		this->species.push_back(new_species);
	}
	else {
		g.inno_func = this->getInnovation;
		species[sp_i].genomes.push_back(g);
	}
}

Genome tournament_selection(vector<Genome>& genomes)
{
	int tournament_size = 2;
	vector<Genome> selected;
	for (int i = 0; i < tournament_size; i++) {
		selected.push_back(genomes[floor(constants::random()*genomes.size())]);
	}
	Genome result = selected[0];
	for (int i = 1; i < selected.size(); i++) {
		if (result.shared_fitness < selected[i].shared_fitness) {
			result = selected[i];
		}
	}
	return result;
};

