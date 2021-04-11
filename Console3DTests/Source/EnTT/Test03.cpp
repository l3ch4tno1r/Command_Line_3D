#include <iostream>

#include <Console3D/External/entt/include/entt.hpp>

class TestCmp1
{
	int a;
};

class TestCmp2
{
	int a;
};

class TestCmp3
{
	int a;
};

union Flag
{
	struct
	{
		bool Cmp1 : 1;
		bool Cmp2 : 1;
		bool Cmp3 : 1;
	};
	unsigned char Result;
};

int main()
{
	entt::registry reg;

	for (int i = 0; i < 100; ++i)
	{
		entt::entity entity = reg.create();

		Flag n;

		n.Result = std::rand() % 8;

		if (n.Cmp1)
			reg.emplace<TestCmp1>(entity);

		if (n.Cmp2)
			reg.emplace<TestCmp2>(entity);

		if (n.Cmp3)
			reg.emplace<TestCmp3>(entity);
	}

	auto group = reg.group<TestCmp1, TestCmp2, TestCmp3>();

	for (entt::entity entity : group)
		std::cout << entt::to_integral(entity) << std::endl;

	std::cin.get();
}