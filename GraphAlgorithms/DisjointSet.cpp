#include "PCH.h"
#include "DisjointSet.h"

DisjointSet::DisjointSet(uint32_t size) : _rank(size + 1, 0), _parent(size + 1)
{
	for (uint32_t i = 0; i < size + 1; i++)
		_parent[i] = i;
}

void DisjointSet::link(uint32_t firstVertex, uint32_t secondVertex)
{
	if (_rank[firstVertex] < _rank[secondVertex])
		_parent[firstVertex] = secondVertex;
	else if (_rank[firstVertex] > _rank[secondVertex])
		_parent[secondVertex] = firstVertex;
	else
	{
		_parent[firstVertex] = secondVertex;
		_rank[secondVertex]++;
	}
}

uint32_t DisjointSet::getRoot(uint32_t vertex)
{
	uint32_t root = vertex;

	while (_parent[root] != root)
		root = _parent[root];

	// Path compression
	while (_parent[vertex] != vertex)
	{
		uint32_t aux = _parent[vertex];
		_parent[vertex] = root;
		vertex = aux;
		_rank[root]--;
	}

	return root;
}

