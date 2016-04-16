#pragma once

#include "PCH.h"
#include "UndirectedGraph.h"

class Tree : public UndirectedGraph
{
	public:
		Tree() : UndirectedGraph() { }
		Tree(uint32_t const& vertices);
		Tree(std::ifstream& ifs);
		Tree(Tree const& source) : UndirectedGraph(source) { }

		uint32_t getDiameter() const;
		uint32_t getRadius() const { return (uint32_t)(getDiameter() / 2); }

		bool isComplete() const override { return false; }
		bool isRegular() const override { return false; }
		bool isConnected() const override { return true; }
		bool isHamiltonian() const override { return false; }
		bool isEulerian() const override { return false; }
		bool isBipartite() const override { return true; }

		Vector<uint32_t> getCenter() const;

		Vector<Vector<bool>> getRoadMatrix() const override { return Vector<Vector<bool>>(_vertices, Vector<bool>(_vertices, true)); }
};

