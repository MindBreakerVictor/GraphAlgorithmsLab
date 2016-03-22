#include "Headers\DirectedGraph.h"

int main()
{
	std::ifstream f("Inputs/in.in");
	const DirectedGraph graph(f);
	
	std::stack<uint> topoSort = graph.getTopologicalSort();

	while (!topoSort.empty())
	{
		std::cout << topoSort.top() << "\n";
		topoSort.pop();
	}

	_getch();
	return 0;
}
