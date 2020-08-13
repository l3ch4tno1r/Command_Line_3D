#include <iostream>
#include <string>

#include "Utilities/InstanceCounter.h"

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
		Test* ptr1 = new Test();
		Test* ptr2 = (Test*)::operator new (sizeof(Test));

		new(ptr2) Test("Joe");

		delete ptr1;

		ptr2->~Test();
		::operator delete(ptr2, sizeof(Test));
	}

	std::cin.get();
}