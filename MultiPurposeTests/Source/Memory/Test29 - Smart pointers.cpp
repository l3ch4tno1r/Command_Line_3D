#include <iostream>
#include <memory>

class Test
{
private:
	uint32_t id;
	static uint32_t count;

public:
	Test() :
		id(++count)
	{}
};

uint32_t Test::count = 0;

int main()
{
	std::weak_ptr<Test> wptr;
	std::shared_ptr<Test> sptr = std::make_shared<Test>();

	std::cout << "1 : " << wptr.use_count() << std::endl;

	{
		wptr = sptr;

		std::cout << "2 : " << wptr.use_count() << std::endl;

		std::shared_ptr<Test> test = wptr.lock();

		std::cout << "3 : " << wptr.use_count() << std::endl;
	}
	
	std::cout << "4 : " << wptr.use_count() << std::endl;

	std::cin.get();
}