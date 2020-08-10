#include <iostream>

template<typename T>
class DynamicArray
{
private:
	T*       m_BeginPtr;
	T*       m_EndPtr;
	uint32_t m_Size;
	uint32_t m_Capacity;

public:
	DynamicArray() :
		m_BeginPtr(nullptr),
		m_EndPtr(nullptr),
		m_Size(0),
		m_Capacity(0)
	{}

	DynamicArray(size_t size)
	{
		m_BeginPtr = new T[size];
		m_EndPtr   = m_BeginPtr + size;

		m_Size     = size;
		m_Capacity = size;
	}

	DynamicArray(size_t size, const T& value)
	{
		m_BeginPtr = new T[size];
		m_EndPtr   = m_BeginPtr + size;

		m_Size     = size;
		m_Capacity = size;

		for (T* ptr = m_BeginPtr; ptr < m_EndPtr; ++ptr)
			*ptr = value;
	}

	~DynamicArray()
	{
		m_Capacity = 0;
		m_Size     = 0;

		delete[] m_BeginPtr;

		m_BeginPtr = nullptr;
	}

	size_t Size() const
	{
		return m_Size;
	}

	T& operator[](size_t i)
	{
		return m_BeginPtr[i];
	}

	const T& operator[](size_t i) const
	{
		return m_BeginPtr[i];
	}
};

template<typename T>
class Counter
{
private:
	uint32_t m_Id;
	static uint32_t m_Count;

public:
	Counter() :
		m_Id(++m_Count)
	{}

	uint32_t Id() const
	{
		return m_Id;
	}

	static uint32_t Count()
	{
		return m_Count;
	}
};

template<typename T>
uint32_t Counter<T>::m_Count = 0;

class Test : public Counter<Test>
{
public:
	Test()
	{
		std::cout << "Hello world" << std::endl;
	}

	Test(const Test& other)
	{
		std::cout << "Hello world - Copy" << std::endl;
	}

	~Test()
	{
		std::cout << "Goodbye world" << std::endl;
	}
};

int main()
{
	{
		DynamicArray<Test> array(5);

		std::cout << Counter<Test>::Count() << std::endl;

		for (size_t i = 0; i < array.Size(); ++i)
			std::cout << "Object #" << array[i].Id() << std::endl;
	}

	std::cin.get();
}