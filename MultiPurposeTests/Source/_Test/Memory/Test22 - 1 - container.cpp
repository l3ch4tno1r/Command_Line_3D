#include <iostream>
#include <utility>
#include <thread>
#include <mutex>

class MemTracker
{
private:
	std::mutex allocated_mut;
	uint64_t allocated;

	std::mutex deallocated_mut;
	uint64_t deallocated;

public:
	MemTracker() :
		allocated(0),
		deallocated(0)
	{}

	~MemTracker()
	{
		std::cout << "Total Memory allocated   : " << allocated   << " bytes." << std::endl;
		std::cout << "Total Memory deallocated : " << deallocated << " bytes." << std::endl;

		std::cout << "\nDelta = " << allocated - deallocated << std::endl;

		std::cin.get();
	}

	void Allocate(size_t size)
	{
		std::unique_lock<std::mutex> lock(allocated_mut);

		allocated += size;
	}

	void Deallocate(size_t size)
	{
		std::unique_lock<std::mutex> lock(deallocated_mut);

		deallocated += size;
	}
};

MemTracker tracker;

void* operator new(size_t size)
{
	tracker.Allocate(size);

	return malloc(size);
}

void operator delete(void* ptr, size_t size)
{
	tracker.Deallocate(size);

	free(ptr);
}

//--------------------//

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

	size_t size;

public:
	LinkedList() :
		first(nullptr),
		last(nullptr),
		size(0)
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

		++size;
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

//----------------------//

using pairii = std::pair<uint32_t, uint32_t>;

std::mutex list_mut;
LinkedList<pairii> list;

void ThreadTest(uint32_t id)
{
	for (uint32_t i = 0; i < 10000; i++)
	{
		//std::unique_lock<std::mutex> lock(list_mut);

		list.PushBack(pairii(id, i));
	}
}

int main()
{
	std::thread t1(ThreadTest, 1);
	std::thread t2(ThreadTest, 2);

	t1.join();
	t2.join();

	/*
	uint32_t count = 0;

	for (auto it = list.Begin(); it != list.End(); ++it)
	{
		std::cout << it->first << ", " << it->second << std::endl;
		count++;
	}

	std::cin.get();
	*/
}