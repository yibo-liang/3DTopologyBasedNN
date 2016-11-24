
#include "Pool.h"
#include <iostream>


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
}

Pool::Pool()
{
	this->innovation = 1;
	this->generation = 0;
}

Pool::~Pool()
{
}

void Pool::addToSpecies(Genome& g)
{
	bool foundSpecies = false;
	
	for (auto si = this->species.begin(); si != this->species.end(); si++) {
	
		//std::cout << si._Ptr <<","<< si->genomes.size() << std::endl;
		Genome president = (*si).genomes.front();
		if (!foundSpecies && g.isSameSpecies(president)) {
			si->genomes.push_back(g);
			foundSpecies = true;
		}

	}
	
	if (!foundSpecies) {
		Species new_species;
		new_species.genomes.push_back(g);
		this->species.push_back(new_species);
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
		if (result.adjusted_fitness < selected[i].adjusted_fitness) {
			result = selected[i];
		}
	}
	return result;
};

