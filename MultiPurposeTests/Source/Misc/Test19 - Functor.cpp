#include <iostream>

class Test
{
public:
	void Method()
	{
		std::cout << "Hello world !" << std::endl;
	}
};

void Display(int a, char c)
{
	std::cout << "Hello world ! " << a << ", " << c << std::endl;
}

template<typename F>
class Func;

template<typename Ret, typename... Args>
class Func<Ret(Args...)>
{
private:
	typedef Ret(*FuncPtr)(Args...);

	FuncPtr f;

public:
	Func(FuncPtr _f) :
		f(_f)
	{}

	Ret operator()(Args... args)
	{
		return (*f)(args...);
	}
};

int main()
{
	/*
	void (Test::*methodptr)() = &Test::Method;

	Test test;

	(test.*methodptr)();
	*/

	Func<void(int, char)> f(Display);

	f(1, 'a');

	std::cin.get();
}