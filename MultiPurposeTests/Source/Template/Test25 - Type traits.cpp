#include <iostream>

template<typename T, T t>
struct IntegralConstant
{
	static constexpr T value = t;

	using ValueType = T;
	using Type = IntegralConstant;

	constexpr operator ValueType() const noexcept
	{
		return value;
	}

	constexpr ValueType operator()() const noexcept
	{
		return value;
	}
};

typedef IntegralConstant<bool, true> TrueType;
typedef IntegralConstant<bool, false> FalseType;

template<typename T1, typename T2>
struct IsSameType : FalseType {};

template<typename T>
struct IsSameType<T, T> : TrueType {};

int main()
{
	std::cout << std::boolalpha;

	std::cout << IsSameType<int, int>::value << std::endl;
	std::cout << IsSameType<int, unsigned int>::value << std::endl;
	std::cout << IsSameType<int, float>() << std::endl;
	std::cout << IsSameType<float, float>() << std::endl;
	std::cout << IsSameType<int, signed int>() << std::endl;

	std::cin.get();
}