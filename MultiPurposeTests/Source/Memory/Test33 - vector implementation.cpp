#include <iostream>
#include <string>

#include "Utilities/InstanceCounter.h"
#include "Utilities/ErrorHandling.h"

#define SEPARATOR(X) std::cout << "\n----------- " << X << " -----------" << std::endl

template<typename T>
class DynamicArray
{
private:
	T*       m_Data     = nullptr;
	uint32_t m_Size     = 0;
	uint32_t m_Capacity = 0;

	void Realloc(size_t capacity)
	{
		T* temp = (T*)::operator new(capacity * sizeof(T));

		if (capacity < m_Size)
			m_Size = capacity;

		for (size_t i = 0; i < m_Size; ++i)
			new(temp + i) T(std::move(m_Data[i]));

		for (size_t i = 0; i < m_Size; ++i)
			m_Data[i].~T();

		::operator delete(m_Data, m_Capacity * sizeof(T));

		m_Data     = temp;
		m_Capacity = capacity;
	}

public:
	DynamicArray()
	{
		Realloc(2);
	}

	DynamicArray(size_t size)
	{
		Realloc(size);
	}

	~DynamicArray()
	{
		Clear();
		::operator delete(m_Data, m_Capacity * sizeof(T));

		m_Capacity = 0;
		m_Size     = 0;

		m_Data = nullptr;
	}

	size_t Size() const
	{
		return m_Size;
	}

	T& operator[](size_t i)
	{
		return m_Data[i];
	}

	const T& operator[](size_t i) const
	{
		return m_Data[i];
	}

	T& PushBack(const T& value)
	{
		if (m_Size >= m_Capacity)
			Realloc(m_Capacity + m_Capacity / 2);
		
		m_Data[m_Size] = value;

		return m_Data[m_Size++];
	}

	template<typename ...Args>
	T& EmplaceBack(Args&& ...args)
	{
		if (m_Size >= m_Capacity)
			Realloc(m_Capacity + m_Capacity / 2);

		new(m_Data + m_Size) T(std::forward<Args>(args)...);

		return m_Data[m_Size++];
	}

	void PopBack()
	{
		if (m_Size > 0)
			m_Data[--m_Size].~T();
	}

	void Clear()
	{
		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		m_Size = 0;
	}

	class Iterator
	{
	private:
		T* m_Ptr;

		Iterator(T* ptr) :
			m_Ptr(ptr)
		{}

		friend class DynamicArray<T>;

	public:
		Iterator() :
			m_Ptr(nullptr)
		{}

		Iterator(const Iterator& other) :
			m_Ptr(other.m_Ptr)
		{}

		T* operator->() { return m_Ptr; }

		T& operator*() { return *m_Ptr; }

		Iterator& operator++()
		{
			++m_Ptr;

			return *this;
		}

		Iterator operator++(int) { return Iterator(m_Ptr++); }

		Iterator& operator--()
		{
			--m_Ptr;

			return *this;
		}

		Iterator operator--(int) { return Iterator(m_Ptr--); }

		Iterator operator+(size_t i) { return Iterator(m_Ptr + i); }
		Iterator operator-(size_t i) { return Iterator(m_Ptr - i); }

		bool operator==(const Iterator& other) const { return m_Ptr == other.m_Ptr; }
		bool operator!=(const Iterator& other) const { return m_Ptr != other.m_Ptr; }
		bool operator< (const Iterator& other) const { return m_Ptr <  other.m_Ptr; }
		bool operator<=(const Iterator& other) const { return m_Ptr <= other.m_Ptr; }
		bool operator> (const Iterator& other) const { return m_Ptr >  other.m_Ptr; }
		bool operator>=(const Iterator& other) const { return m_Ptr >= other.m_Ptr; }
	};

	Iterator Begin()
	{
		return Iterator(m_Data);
	}

	Iterator End()
	{
		return Iterator(m_Data + m_Size);
	}

private:
	void MoveMemBlockBackward(T* dest, T* src)
	{
		ASSERT(dest >= m_Data && dest < m_Data + m_Size);
		ASSERT(src  >= m_Data && src  < m_Data + m_Size);
		ASSERT(dest < src);

		for (T* ptr = dest; ptr < src; ++ptr)
			ptr->~T();

		size_t delta = src - dest;
		m_Size -= delta;

		for (T* ptr = dest; ptr < m_Data + m_Size; ptr++)
			*ptr = std::move(*(ptr + delta));
	}

	void MoveMemBlockForward(T* dest, T* src)
	{
		ASSERT(dest >= m_Data && dest < m_Data + m_Size);
		ASSERT(src  >= m_Data && src  < m_Data + m_Size);
		ASSERT(dest >  src);

		size_t delta = dest - src;

		if (m_Size + delta > m_Capacity)
			Realloc(m_Capacity + m_Capacity / 2);

		ASSERT(m_Size + delta <= m_Capacity);

		for (T* ptr = m_Data + m_Size - 1; ptr >= src; --ptr)
			*(ptr + delta) = std::move(*ptr);

		m_Size += delta;
	}

public:
	void Erase(const Iterator& it)
	{
		MoveMemBlockBackward(it.m_Ptr, it.m_Ptr + 1);
	}

	void Erase(const Iterator& first, const Iterator& last)
	{
		MoveMemBlockBackward(first.m_Ptr, last.m_Ptr);
	}

	void Insert(const Iterator& it, const T& value)
	{
		MoveMemBlockForward(it.m_Ptr + 1, it.m_Ptr);

		*(it.m_Ptr) = value;
	}

	void Insert(const Iterator& it, T&& value)
	{
		MoveMemBlockForward(it.m_Ptr + 1, it.m_Ptr);

		*(it.m_Ptr) = std::move(value);
	}
};

class Test : public Counter<Test>
{
private:
	std::string m_Name;

public:
	Test() :
		m_Name("No name")
	{
		std::cout << "#" << this->Id() << " Hello world " << m_Name << std::endl;
	}

	Test(const char* str) :
		m_Name(str)
	{
		std::cout << "#" << this->Id() << " Hello world " << m_Name << std::endl;
	}

	Test(const Test& other) :
		m_Name(other.m_Name)
	{
		std::cout << "#" << this->Id() << " Hello world - Copy " << m_Name << std::endl;
	}

	Test(Test&& other) :
		m_Name(std::move(other.m_Name))
	{
		std::cout << "#" << this->Id() << " Hello world - Move " << m_Name << std::endl;
	}

	~Test()
	{
		std::cout << "#" << this->Id() << " Goodbye world " << m_Name << std::endl;
	}

	Test& operator=(const Test& other)
	{
		std::cout << "#" << this->Id() << " copy assigned " << m_Name << std::endl;

		m_Name = other.m_Name;

		return *this;
	}

	Test& operator=(Test&& other)
	{
		m_Name = std::move(other.m_Name);

		std::cout << "#" << this->Id() << " move assigned " << this->Name() << std::endl;

		return *this;
	}

	const std::string& Name() const
	{
		return m_Name;
	}
};

int main()
{
	{
		DynamicArray<Test> vec;

		SEPARATOR("Add Joe");
		vec.EmplaceBack("Joe");
		SEPARATOR("Add Jack");
		vec.EmplaceBack("Jack");
		SEPARATOR("Add William");
		vec.EmplaceBack("William");
		SEPARATOR("Add Averell");
		vec.EmplaceBack("Averell");

		SEPARATOR("Display");

		for (size_t i = 0; i < vec.Size(); ++i)
			std::cout << vec[i].Id() << " - " << vec[i].Name() << std::endl;

		SEPARATOR("Pop");

		vec.PopBack();

		for (size_t i = 0; i < vec.Size(); ++i)
			std::cout << vec[i].Id() << " - " << vec[i].Name() << std::endl;

		SEPARATOR("Clear");

		vec.Clear();

		vec.EmplaceBack("Joe");
		vec.EmplaceBack("Jack");
		vec.EmplaceBack("William");
		vec.EmplaceBack("Averell");
		vec.EmplaceBack("Matt");

		SEPARATOR("Iterator 1");

		for(auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "Iterator test : " << it->Name() << std::endl;

		SEPARATOR("Iterator 2");

		auto it1 = vec.Begin();
		auto it2 = it1++;

		std::cout << "Iterator test : " << it1->Name() << std::endl;
		std::cout << "Iterator test : " << it2->Name() << std::endl;
		std::cout << "Iterator test : " << (it1 + 2)->Name() << std::endl;
		std::cout << "Iterator test : " << (vec.End() - 1)->Name() << std::endl;

		SEPARATOR("Erase 1");

		for (auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "Before erase : " << it->Name() << std::endl;

		vec.Erase(vec.Begin() + 2);

		for (auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "After erase : " << it->Name() << std::endl;

		SEPARATOR("Insert");

		vec.Insert(vec.Begin() + 2, Test("William"));
		//vec.Insert(vec.End(),       Test("Melody"));

		for (auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "After insert : " << it->Name() << std::endl;

		SEPARATOR("Erase 2");

		vec.Erase(vec.Begin() + 1, vec.Begin() + 3);

		for (auto it = vec.Begin(); it != vec.End(); ++it)
			std::cout << "After insert : " << it->Name() << std::endl;

		SEPARATOR("End");
	}

	std::cin.get();
}