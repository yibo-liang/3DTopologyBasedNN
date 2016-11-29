
#include "Pool.h"
#include "globals.h"
#include <iostream>
#include <algorithm>

using namespace constants;



struct better_fitness {
	inline bool operator() (const Genome& g1, const Genome& g2) {
		return (g1.shared_fitness > g2.shared_fitness);
	}
};

Genome tournament_selection(vector<Genome>& genomes)
{
	int tournament_size = 2;
	vector<Genome> selected;
	for (int i = 0; i < tournament_size; i++) {
		selected.push_back(genomes[floor(constants::random()*genomes.size())]);
	}
	Genome& result = selected[0];
	for (int i = 1; i < selected.size(); i++) {
		if (result.shared_fitness < selected[i].shared_fitness) {
			result = selected[i];
		}
	}
	return result;
};


void Pool::newGeneration()
{
	//for each species
	//for each genome, create a network
	//for each network, eval it.
	//eval(func, fcun){ }
	//selection + crossover
	//mutation
	//eliticism
	double max_fitness = 0;
	double max_shared_fitness = 0;
	double fit_sum = 0;
#pragma omp parallel
	for (int sp_i = 0; sp_i < species.size(); sp_i++) {
		Species& sp = species[sp_i];
#pragma omp for schedule(dynamic,1)
		for (int g_i = 0; g_i < sp.genomes.size(); g_i++) {
			Genome& g = sp.genomes[g_i];
			g.network = g.toNeuralNetwork();
			g.fitness = evaluate_network_on_each_node(g.network, inputs, outputs, DEFAULT_INPUT_STEP, DEFAULT_OUTPUT_STEP);
			g.shared_fitness = g.fitness / (double)sp.genomes.size();
			if (g.fitness > max_fitness) max_fitness = g.fitness;
			if (g.shared_fitness > max_shared_fitness) max_shared_fitness = g.shared_fitness;
		}

	}
#pragma omp parallel for reduction(+:fit_sum)
	for (int i = 0; i < species.size(); i++) {
		Species& sp = species[i];
		std::sort(sp.genomes.begin(), sp.genomes.end(), better_fitness());
		fit_sum += sp.genomes[0].shared_fitness;
	}

	vector<Genome> new_generation;

	//if elitism
	if (this->configuration.is_elitism)
		for (int sp_i = 0; sp_i < species.size(); sp_i++) {
			new_generation.push_back(species[sp_i].genomes[0]);
		}

	int population = this->configuration.pool_population - new_generation.size();

	for (int sp_i = 0; sp_i < species.size(); sp_i++) {
		Species& sp = species[sp_i];
		int n = sp.genomes[0].shared_fitness / fit_sum * population; // the number of genome from this species in next generation
		if (n < 1 && sp.staleness> 15) n = 1;
		for (int i = 0; i < n; i++) {

			vector<Genome> gs;
			//if adoption
			if (random() < this->adoption_rate) {
				int alien_si = (int)(random()*species.size());
				gs.push_back( tournament_selection(species[alien_si].genomes));
			}
			else {
				gs.push_back(tournament_selection(sp.genomes));
			}
			gs.push_back( tournament_selection(sp.genomes));

			if (random() < this->configuration.probabilities["crossover"]) {
				Genome g = fromCrossOver(gs[0], gs[1]);
				new_generation.push_back(g);
			}
			else {
				new_generation.push_back(random() > 0.5 ? gs[0] : gs[1]);
			}
		}
	}
	//mutation and add to next generation
	this->species.clear();
	for (int i = 0; i < new_generation.size(); i++) {
		Genome& g = new_generation[i];
		this->addToSpecies(fromMutate(g));
	}
	this->generation++;


	cout << "Generation done: " << this->generation <<" max fit="<<max_fitness<<", shared="<<max_shared_fitness<< endl;
	for (int i = 0; i < species.size(); i++) {
		cout << i << ", " << species[i].genomes.size()<<endl;
	}
}

Pool::Pool(const Pool & obj)
{
	this->species = obj.species;
	this->outputs = obj.outputs;
	this->inputs = obj.inputs;
	this->innovation = obj.innovation;
//	this->GASelection = obj.GASelection;
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

	for (int si = 0; si < species.size(); si++) {
		//std::cout << si._Ptr <<","<< si->genomes.size() << std::endl;
		int random_id = (int)(random()* species[si].genomes.size());
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

	if (sp_i == -1) {
		Species new_species;
		g.pool_id = this->pool_id;
		new_species.genomes.push_back(g);
		this->species.push_back(new_species);
	}
	else {
		g.pool_id = this->pool_id;
		species[sp_i].genomes.push_back(g);
	}
}

void Pool::set_inputs(vector<vector<double>> inputs)
{
	this->inputs = inputs;
}

void Pool::set_outputs(vector<vector<double>> outputs)
{
	this->outputs = outputs;
}
