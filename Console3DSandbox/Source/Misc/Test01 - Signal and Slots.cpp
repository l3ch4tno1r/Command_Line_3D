#include <iostream>
#include <string>

#include <Console3D/Source/Event/SignalSlot.h>
#include <Utilities/Source/InstanceCounter.h>

class Test : public Counter<Test>
{
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

	void Display() const { std::cout << this->Name() << std::endl; }

private:
	std::string m_Name;
};

class Emmiter
{
public:
	LCN::Signal<void()> SignalEmmit;
};

class Receiver : public Counter<Receiver>
{
public:
	Receiver() :
		SlotDisplay(INIT_SLOT(Receiver::Display))
	{}

	virtual void Display() = 0;
	LCN::Slot<Receiver, void()> SlotDisplay;
};

class ReceiverImpl : public Receiver
{
public:
	void Display() override { std::cout << this->Id() << " Hello world !" << std::endl; }
};

int main()
{
	{
		Emmiter      emmiter;
		ReceiverImpl receiver1;
		ReceiverImpl receiver2;
		ReceiverImpl receiver3;

		receiver2.SlotDisplay();

		LCN::Bind(emmiter.SignalEmmit, receiver1.SlotDisplay);
		LCN::Bind(emmiter.SignalEmmit, receiver2.SlotDisplay);
		LCN::Bind(emmiter.SignalEmmit, receiver3.SlotDisplay);

		emmiter.SignalEmmit.Emmit();
	}

	std::cin.get();
}