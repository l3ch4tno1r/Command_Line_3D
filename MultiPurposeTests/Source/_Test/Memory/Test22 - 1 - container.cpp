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

		friend class StaticArray<T, N>;

	public:
		Iterator(const Iterator& other) :
			ptr(other.ptr)
		{}

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

		Iterator& operator--()
		{
			ptr--;

			return *this;
		}

		Iterator operator--(int)
		{
			T* temp = ptr--;

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

template<typename T>
class LinkedList
{
private:
	struct LinkedListItem
	{
		LinkedListItem* next;
		T data;

		LinkedListItem(const T& data) :
			next(nullptr),
			data(data)
		{}
	};

	LinkedListItem* first;
	LinkedListItem* last;

public:
	LinkedList() :
		first(nullptr),
		last(nullptr)
	{}

	~LinkedList()
	{
		LinkedListItem* ptr = first;

		while (ptr != nullptr)
		{
			LinkedListItem* next = ptr->next;

			delete ptr;

			ptr = next;
		}
	}

	void PushBack(const T& data)
	{
		LinkedListItem* item = new LinkedListItem(data);

		if (first == nullptr)
			last = first = item;
		else
		{
			last->next = item;

			last = item;
		}
	}

	class Iterator
	{
	private:
		LinkedListItem* ptr;

		Iterator(LinkedListItem* ptr) :
			ptr(ptr)
		{}

		friend class LinkedList<T>;

	public:
		Iterator& operator++()
		{
			ptr = ptr->next;

			return *this;
		}

		T& operator*()
		{
			return ptr->data;
		}

		T* operator->()
		{
			return &ptr->data;
		}

		bool operator!=(const Iterator& other)
		{
			return ptr != other.ptr;
		}
	};

	Iterator Begin()
	{
		return Iterator(first);
	}

	Iterator End()
	{
		return Iterator(nullptr);
	}
};

class Test
{
private:
	static uint32_t count;
	uint32_t id;

public:
	Test() : id(++count)
	{
		std::cout << "Hello world ! " << id << std::endl;
	}

	Test(const Test&) : id(++count)
	{
		std::cout << "Hello world copy ! " << id << std::endl;
	}

	~Test()
	{
		std::cout << "Goodbye world ! " << id << std::endl;
	}

	void Display() const
	{
		std::cout << "Display ! " << id << std::endl;
	}
};

uint32_t Test::count = 0;

int main()
{
	{
		LinkedList<Test> list;

		list.PushBack(Test());
		list.PushBack(Test());
		list.PushBack(Test());
		list.PushBack(Test());

		for (auto it = list.Begin(); it != list.End(); ++it)
			it->Display();
	}

	std::cin.get();
}