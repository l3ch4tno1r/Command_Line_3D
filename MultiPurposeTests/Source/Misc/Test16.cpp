#include <iostream>
#include <unordered_set>
#include <utility>

using Edge = std::pair<unsigned int, unsigned int>;

struct HashEdge
{
	size_t operator()(const Edge& edge) const
	{
		return std::hash<unsigned int>()(edge.first + edge.second);
	}
};

int main()
{
	std::unordered_set<Edge, HashEdge> edges;

	Edge edge1 = { 1, 2 };
	Edge edge2 = { 2, 1 };

	edges.insert(edge1);

	std::cout << edges.count(edge1) << std::endl;
	std::cout << edges.count(edge2) << std::endl;

	if(edges.count(edge2) == 0)
		edges.insert(edge2);

	std::cin.get();
}
