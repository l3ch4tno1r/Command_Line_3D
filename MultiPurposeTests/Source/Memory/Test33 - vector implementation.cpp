#include <iostream>
#include <string>

#include "Utilities/InstanceCounter.h"

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
		DynamicArray<Test> array;

		std::cout << "------------- Add Joe --------------" << std::endl;
		array.EmplaceBack("Joe");
		std::cout << "------------- Add Jack --------------" << std::endl;
		array.EmplaceBack("Jack");
		std::cout << "------------- Add William --------------" << std::endl;
		array.EmplaceBack("William");
		std::cout << "------------- Add Averell --------------" << std::endl;
		array.EmplaceBack("Averell");

		std::cout << "------------- Display --------------" << std::endl;

		for (size_t i = 0; i < array.Size(); ++i)
			std::cout << array[i].Id() << " - " << array[i].Name() << std::endl;

		std::cout << "------------- Pop --------------" << std::endl;

		array.PopBack();

		for (size_t i = 0; i < array.Size(); ++i)
			std::cout << array[i].Id() << " - " << array[i].Name() << std::endl;

		std::cout << "------------- Clear --------------" << std::endl;

		array.Clear();

		array.EmplaceBack("Matt");

		std::cout << "---------------------------" << std::endl;
	}

	std::cin.get();
}