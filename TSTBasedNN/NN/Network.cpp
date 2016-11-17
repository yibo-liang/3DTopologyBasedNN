
#include "Network.h"

Network::Network(const Network & obj)
{
	for (auto it = this->edges.begin(); it != this->edges.end(); it++) {
		this->edges[it->first] = Edge(it->second);
	}
	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {
		this->nodes[it->first] = Node(it->second);
	}
}

Network::Network()
{
}

Network::~Network()
{
	//for (auto it = this->edges.begin(); it != this->edges.end(); it++) {

	//	delete &it->second;
	//}
	//for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {

	//	delete &it->second;
	//}
}
