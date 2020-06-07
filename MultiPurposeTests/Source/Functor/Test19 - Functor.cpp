#include <iostream>
#include <functional>

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

template<class C, class M>
class MemFunc;

template<class C, class Ret, class... Args>
class MemFunc<C, Ret(Args...)>
{
private:
	typedef Ret(C::*MemFuncPtr)(Args...);

	C* o;
	MemFuncPtr m;

public:
	MemFunc(C* _o, MemFunc _m) :
		o(_o),
		m(_m)
	{}

	Ret operator()(Args... args)
	{
		return (o.*m)(args...);
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

	Test test;

	std::function<void(void)> func = std::bind(&Test::Method, &test);

	func();

	//MemFunc<Test, void(void)> m(&test, &Test::Method);

	std::cin.get();
}