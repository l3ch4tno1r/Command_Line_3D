#include <iostream>
#include <mutex>

class TestLockable
{
public:
	void lock()
	{
		std::cout << "Locked ! " << std::endl;
	}

	void unlock()
	{
		std::cout << "Unlocked ! " << std::endl;
	}
};

int main()
{
	TestLockable test;

	{
		std::lock_guard<TestLockable> lock(test);

		std::cout << "Test" << std::endl;
	}

	const TestLockable& ref = std::cref(test);

	std::cin.get();
}