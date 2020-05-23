#include <iostream>

class RValue
{
private:
	RValue() = default;
	RValue(const RValue&) = default;
	RValue& operator=(const RValue&) = default;

	friend class Test;
};

template<typename T>
RValue& operator<<(RValue& stream, const T& value)
{
	stream << value;

	return stream;
}

class Test
{
public:
	operator RValue()
	{
		return RValue();
	}
};

int main()
{
	Test() << "Hello world";

	std::cin.get();
}