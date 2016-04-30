#ifndef _DISJOINT_SET_H
#define _DISJOINT_SET_H

#include "PCH.h"

class DisjointSet
{
	public:
		explicit DisjointSet(uint32_t const& size);
		DisjointSet(DisjointSet const& source) : _rank(source._rank), _parent(source._parent) { }

		void Link(uint32_t const& firstVertex, uint32_t const& secondVertex);
		void UnionSets(uint32_t const& firstVertex, uint32_t const& secondVertex) { Link(GetRoot(firstVertex), GetRoot(secondVertex)); }

		uint32_t GetRoot(uint32_t vertex);

	private:
		Vector<uint32_t> _rank, _parent;
};

#endif
