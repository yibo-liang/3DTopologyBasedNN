
#include "Network.h"

Network::Network()
{
}

Network::~Network()
{
	for (auto it = this->edges.begin(); it != this->edges.end(); it++) {

		delete &it->second;
	}
	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {

		delete &it->second;
	}
}
