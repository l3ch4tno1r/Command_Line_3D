#include <iostream>

template<typename T, uint32_t N>
class StaticArray
{
private:
	T tab[N];

public:
	T& operator[](uint32_t i)
	{
		return tab[i];
	}

	const T& operator[](uint32_t i) const
	{
		return tab[i];
	}

	uint32_t Size() const
	{
		return N;
	}

	class Iterator
	{
	private:
		T* ptr;

		Iterator(T* ptr) :
			ptr(ptr)
		{}

		friend class StaticArray;

	public:
		Iterator& operator++()
		{
			ptr++;

			return *this;
		}

		Iterator operator++(int)
		{
			T* temp = ptr++;

			return Iterator(temp);
		}

		T& operator*()
		{
			return *ptr;
		}

		T* operator->()
		{
			return ptr;
		}

		bool operator!=(const Iterator& other)
		{
			return ptr != other.ptr;
		}
	};

	Iterator Begin()
	{
		return Iterator(tab);
	}

	Iterator End()
	{
		return Iterator(tab + N);
	}
};

class Test
{
private:
	static uint32_t count;
	uint32_t id;

public:
	Test() : id(++count)
	{}

	void Display() const
	{
		std::cout << "Hello world ! " << id << std::endl;
	}
};

uint32_t Test::count = 0;

int main()
{
	StaticArray<Test, 4> sarray;

	for (auto it = sarray.Begin(); it != sarray.End(); it++)
		it->Display();

	std::cin.get();
}