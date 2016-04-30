#ifndef _TREE_H
#define _TREE_H

#include "PCH.h"
#include "UndirectedGraph.h"

class Tree : public UndirectedGraph
{
	public:
		Tree() : UndirectedGraph() { }
		explicit Tree(uint32_t const& vertices);
		explicit Tree(std::ifstream& ifs);
		Tree(Tree const& source) : UndirectedGraph(source) { }

		uint32_t GetDiameter() const;
		uint32_t GetRadius() const;

		bool IsComplete() const override { return false; }
		bool IsRegular() const override { return false; }
		bool IsConnected() const override { return true; }
		bool IsHamiltonian() const override { return false; }
		bool IsEulerian() const override { return false; }
		bool IsBipartite() const override { return true; }

		Vector<uint32_t> GetCenter() const;

		Vector<Vector<bool>> GetRoadMatrix() const override { return Vector<Vector<bool>>(GetVertices(), Vector<bool>(GetVertices(), true)); }
};

#endif

