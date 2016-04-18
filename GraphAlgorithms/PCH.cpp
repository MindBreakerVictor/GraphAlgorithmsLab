#include "PCH.h"

bool EDGES_PAIR_COMPARE(Pair<Pair<uint32_t, uint32_t>, int32_t> const& first,
	Pair<Pair<uint32_t, uint32_t>, int32_t> const& second)
{
	if (first.second < second.second)
		return true;

	return false;
}

