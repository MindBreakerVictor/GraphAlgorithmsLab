#pragma once

#include <fstream>
#include <iostream>

#include <ctime>

#include <stack>
#include <queue>
#include <vector>
#include <algorithm>

template <class _Type>
using Vector = std::vector<_Type>;

template <class _Type>
using Queue = std::queue<_Type>;

template <class _Type>
using Stack = std::stack<_Type>;

template <class _Type1, class _Type2>
using Pair = std::pair<_Type1, _Type2>;

bool EDGES_PAIR_COMPARE(Pair<Pair<uint32_t, uint32_t>, int32_t> const& first,
	Pair<Pair<uint32_t, uint32_t>, int32_t> const& second);

