#include <iostream>
#include <memory>

class Base
{
public:
	Base()
	{
		std::cout << "Base constructed !" << std::endl;
	}

	virtual ~Base()
	{
		std::cout << "Base destructed !" << std::endl;
	}

	virtual void Display() = 0;
};

class Derived : public Base
{
public:
	Derived()
	{
		std::cout << "Derived constructed !" << std::endl;
	}

	~Derived()
	{
		std::cout << "Derived destructed !" << std::endl;
	}

	void Display()
	{
		std::cout << "I am derived" << std::endl;
	}
};

int main()
{
	{
		std::shared_ptr<Base> ptr = std::make_shared<Derived>();

		ptr->Display();

		std::weak_ptr<Derived> weak = std::dynamic_pointer_cast<Derived>(ptr);

		weak.lock()->Display();
	}

	std::cin.get();
}