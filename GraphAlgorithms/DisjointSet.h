#ifndef _DISJOINT_SET_H
#define _DISJOINT_SET_H

#include "PCH.h"

class DisjointSet
{
	public:
		explicit DisjointSet(uint32_t size);
		DisjointSet(DisjointSet const& source) : _rank(source._rank), _parent(source._parent) { }

		void link(uint32_t firstVertex, uint32_t secondVertex);
		void unionSets(uint32_t firstVertex, uint32_t secondVertex) { link(getRoot(firstVertex), getRoot(secondVertex)); }

		uint32_t getRoot(uint32_t vertex);

	private:
		Vector<uint32_t> _rank, _parent;
};

#endif
