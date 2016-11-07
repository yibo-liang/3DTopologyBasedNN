
#include "Pool.h"

Pool::Pool()
{
}

Pool::~Pool()
{
}

void Pool::addToSpecies(Genome g)
{
	bool foundSpecies = false;
	typedef list<Species> species_list;
	for (species_list::const_iterator ci = this->species.begin(); ci != this->species.end(); ci++) {
		Species s = *ci;
		Genome president = *s.genomes.begin();
		if (!foundSpecies && g.isSameSpecies(president)) {
			s.genomes.push_back(g);
			foundSpecies = true;
		}

	}
	if (!foundSpecies) {
		Species new_species;
		new_species.genomes.push_back(g);
		this->species.push_back(new_species);
	}

}
