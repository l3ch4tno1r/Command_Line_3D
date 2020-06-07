#include <iostream>

class AbstractNode
{
public:
	virtual uint32_t GetLevel() const = 0;

	virtual const AbstractNode& GetLeftChild() const = 0;

	virtual const AbstractNode& GetRightChild() const = 0;

	virtual void SayHi() const = 0;
};

template<uint32_t N>
class StaticBinaryTreeNode : public AbstractNode
{
private:
	StaticBinaryTreeNode<N - 1> LHS;
	StaticBinaryTreeNode<N - 1> RHS;

public:
	StaticBinaryTreeNode(int lower, int upper) :
		LHS(lower, (upper + lower) / 2),
		RHS((upper + lower) / 2, upper)
	{}

	const AbstractNode& GetLeftChild() const override
	{
		return LHS;
	}

	const AbstractNode& GetRightChild() const override
	{
		return RHS;
	}

	uint32_t GetLevel() const override
	{
		return N;
	}

	void SayHi() const override
	{
		std::cout << "Hello I am a node !" << std::endl;
	}
};

template<>
class StaticBinaryTreeNode<0> : public AbstractNode
{
private:
	int m_Lower;
	int m_Upper;

public:
	StaticBinaryTreeNode(int lower, int upper) :
		m_Lower(lower),
		m_Upper(upper)
	{}

	const AbstractNode& GetLeftChild() const override
	{
		throw std::exception("End");
	}

	const AbstractNode& GetRightChild() const override
	{
		throw std::exception("End");
	}

	uint32_t GetLevel() const override
	{
		return 0;
	}

	void SayHi() const override
	{
		std::cout << "Hello I am a leaf !" << std::endl;
	}
};

int main()
{
	StaticBinaryTreeNode<3> tree(0, 100);

	const AbstractNode* node = &tree;

	while (node->GetLevel() != 0)
	{
		node->SayHi();

		node = &node->GetLeftChild();
	}

	std::cin.get();
}