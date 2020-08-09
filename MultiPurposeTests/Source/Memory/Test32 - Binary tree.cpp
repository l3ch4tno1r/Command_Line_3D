#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

#include <cstdlib>
#include <ctime>

#include "Utilities/ErrorHandling.h"

std::vector<size_t> histogram(50, size_t(0));

template<typename T>
class Tree
{
private:
	struct TreeNode
	{
		static const size_t Left  = 0;
		static const size_t Right = 1;

		T m_Data;

		TreeNode* m_Children[2];

		TreeNode(const T& data) :
			m_Data(data)
		{
			m_Children[Left]  = nullptr;
			m_Children[Right] = nullptr;
		}

		~TreeNode()
		{
			delete m_Children[Left];
			//std::cout << m_Data << " deleted !" << std::endl;
			delete m_Children[Right];
		}

		void DisplayRecursive(size_t indent = 0) const
		{
			if (m_Children[Left])
				m_Children[Left]->DisplayRecursive(indent + 1);

			std::cout << indent << "\t|" << std::string(1 * indent, '-') << m_Data << std::endl;

			if (m_Children[Right])
				m_Children[Right]->DisplayRecursive(indent + 1);

			ASSERT(indent < histogram.size());

			histogram[std::min(indent, histogram.size() - 1)]++;
		}
	};

	size_t    m_Size;
	TreeNode* m_Root;

public:
	Tree() :
		m_Root(nullptr)
	{}

	~Tree()
	{
		delete m_Root;
	}

	void Insert(const T& value)
	{
		TreeNode* elementToInsert = new TreeNode(value);

		if (!m_Root)
		{
			m_Root = elementToInsert;
			++m_Size;
			return;
		}

		TreeNode* currentNode = m_Root;

		while (true)
		{
			if (value == currentNode->m_Data)
				break;

			size_t child;

			if (value < currentNode->m_Data)
				child = TreeNode::Left;
			else
				child = TreeNode::Right;

			if (!currentNode->m_Children[child])
			{
				currentNode->m_Children[child] = elementToInsert;
				++m_Size;
				break;
			}
			else
				currentNode = currentNode->m_Children[child];
		}
	}

	size_t Size() const { return m_Size; }

	void Display() const
	{
		if (m_Root)
			m_Root->DisplayRecursive();
	}
};

int main()
{
	std::random_device rd;
	std::mt19937 g(rd());

	std::vector<int> v(4 * 10);

	for (int i = 0; i < v.size(); ++i)
		v[i] = i;

	std::shuffle(v.begin(), v.end(), g);

	Tree<int> tree;

	for (int e : v)
		tree.Insert(e);

	tree.Display();

	std::cout << "\n---------- Size : " << tree.Size() << " ----------\n" << std::endl;

	size_t x   = 0;
	size_t idx = 0;

	for (size_t& e : histogram)
	{
		std::cout << e << ' ';

		x += ++idx * e;
	}

	std::cout << "\n\nAverage no of access : " << x / v.size() << std::endl;

	std::cin.get();
}