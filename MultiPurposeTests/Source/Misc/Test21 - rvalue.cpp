#include <iostream>

class RValue
{
private:
	RValue()
	{
		std::cout << "RValue Hello World !" << std::endl;
	}

	RValue(const RValue&) = default;
	RValue& operator=(const RValue&) = default;

	~RValue()
	{
		std::cout << "RValue Goodbye World !" << std::endl;
	}

	friend RValue rValue();
};

RValue rValue()
{
	return RValue();
}

int main()
{
	rValue();

	std::cin.get();
}