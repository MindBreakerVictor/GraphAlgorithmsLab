#ifndef _PCH_H
#define _PCH_H

#include <fstream>
#include <iostream>

#include <ctime>

#include <stack>
#include <queue>
#include <vector>
#include <algorithm>

template <class _Type>
using Queue = std::queue<_Type>;

template <class _Type>
using Stack = std::stack<_Type>;

template <class _Type>
using Vector = std::vector<_Type>;

template <class _Type>
using Matrix = Vector<Vector<_Type>>;

template <class _Type1, class _Type2>
using Pair = std::pair<_Type1, _Type2>;

template <class _Type, class _Container = Vector<_Type>, class _Predicate = std::less<_Container::value_type>>
using PriorityQueue = std::priority_queue<_Type, _Container, _Predicate>;

#endif

