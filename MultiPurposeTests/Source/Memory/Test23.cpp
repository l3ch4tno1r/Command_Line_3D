#include <iostream>

class Singleton
{
private:
	uint32_t id;
	static uint32_t count;

	Singleton() : id(++count)
	{}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

public:
	static Singleton& Get()
	{
		static Singleton instance;
		return instance;
	}
};

uint32_t Singleton::count = 0;

int main()
{
	Singleton& ref = Singleton::Get();
	Singleton* ptr = (Singleton*)malloc(sizeof(Singleton));

	memcpy(ptr, &ref, sizeof(Singleton));

	std::cin.get();
}