#include <iostream>

template<uint32_t N>
struct Factorielle
{
	enum { valeur = N * Factorielle<N - 1>::valeur };
};

template<>
struct Factorielle<0>
{
	enum { valeur = 1 };
};

/////////////////
//-- Counter --//
/////////////////

template<class C>
class Counter
{
private:
	static int count;

public:
	Counter()
	{
		++count;
	}

	Counter(const Counter&)
	{
		++count;
	}

	static int Count()
	{
		return count;
	}
};

template<class C>
int Counter<C>::count = 0;

class Test : public Counter<Test>
{
private:
	int id;

public:
	Test() :
		id(Counter<Test>::Count())
	{
		std::cout << "default ctor " << id << std::endl;
	}

	Test(const Test&) :
		id(Counter<Test>::Count())
	{
		std::cout << "copy ctor " << id << std::endl;
	}

	Test(Test&&) :
		id(Counter<Test>::Count())
	{
		std::cout << "move ctor " << id << std::endl;
	}
};

int main()
{
	Test test1;
	Test test2 = test1;
	Test test3 = std::move(test1);

	std::cout << Test::Count() << std::endl;

	std::cin.get();
}