#include <iostream>
#include <unordered_set>
#include <utility>

#define SEPARATOR(X) std::cout << "------- " << X << " -------" << std::endl

void* operator new(size_t size)
{
	std::cout << "Allocating " << size << " bytes" << std::endl;

	return malloc(size);
}

void operator delete(void* ptr, size_t size)
{
	std::cout << "Deallocating " << size << " bytes" << std::endl;

	free(ptr);
}

using Edge = std::pair<unsigned int, unsigned int>;

struct HashEdge
{
	size_t operator()(const Edge& edge) const
	{
		return std::hash<unsigned int>()(edge.first + edge.second);
	}
};

struct EqualEdge
{
	bool operator()(const Edge& e1, const Edge& e2) const
	{
		return (e1.first == e2.first  && e1.second == e2.second) ||
			   (e1.first == e2.second && e1.second == e2.first);
	}
};

int main()
{
	std::unordered_set<Edge, HashEdge, EqualEdge> edges;

	SEPARATOR(1);

	Edge edge1 = { 1, 2 };
	Edge edge2 = { 2, 1 };
	Edge edge3 = { 1, 3 };

	std::cout << "edge1 == edge2 ? " << EqualEdge()(edge1, edge2) << std::endl;
	std::cout << "edge1 == edge1 ? " << EqualEdge()(edge1, edge1) << std::endl;
	std::cout << "edge1 == edge3 ? " << EqualEdge()(edge1, edge3) << std::endl;

	SEPARATOR(2);
	edges.insert(edge1);
	SEPARATOR(3);
	edges.insert(edge2);
	SEPARATOR(4);
	edges.insert(edge3);

	SEPARATOR(5);

	std::cout << edges.count(edge1) << std::endl;
	std::cout << edges.count(edge2) << std::endl;
	std::cout << edges.count(edge3) << std::endl;

	for (auto it = edges.begin(); it != edges.end(); it++)
		std::cout << it->first << ", " << it->second << std::endl;

	std::cin.get();
}
