
#include "Pool.h"
#include <iostream>

Pool::Pool()
{
}

Pool::~Pool()
{
}

void Pool::addToSpecies(Genome* g)
{
	bool foundSpecies = false;
	typedef list<Species> species_list;
	for (species_list::iterator si = this->species.begin(); si != this->species.end(); si++) {
	
		std::cout << si._Ptr <<","<< si->genomes.size() << std::endl;
		Genome* president = &(*si).genomes.front();
		if (!foundSpecies && g->isSameSpecies(*president)) {
			si->genomes.push_back(*g);
			foundSpecies = true;
		}

	}
	
	if (!foundSpecies) {
		Species* new_species=new Species();
		new_species->genomes.push_back(*g);
		this->species.push_back(*new_species);
	}

}
