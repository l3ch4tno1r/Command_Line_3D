#ifdef TEST02

#include <iostream>
#include <vector>
#include <array>
#include <thread>

using namespace std;

#include "SMatrix.h"

#include "Utilities\TimeMeasurement.h"

class Test
{
private:
	uint32_t m_Id;
	static uint32_t s_Count;
public:
	Test() : m_Id(++s_Count)
	{
		cout << m_Id << " : Default Constructor" << endl;
	}

	Test(const Test&) : m_Id(++s_Count)
	{
		cout << m_Id << " : Copy Constructor" << endl;
	}

	Test(const Test&&) : m_Id(++s_Count)
	{
		cout << m_Id << " : Move Constructor" << endl;
	}

	~Test()
	{
		cout << m_Id << " : Destructor" << endl;
	}
};

uint32_t Test::s_Count = 0;

void TestStackAlloc()
{
	for (int i = 0; i < 1000000; i++)
		int tab[10];
}

void TestHeapAlloc()
{
	for (int i = 0; i < 1000000; i++)
	{
		int* tab = new int[10];
		delete[] tab;
	}
}

void TestStdArray()
{
	for (int i = 0; i < 1000000; i++)
		array<int, 10>();
}

void TestStdVector()
{
	for (int i = 0; i < 1000000; i++)
		vector<int>(10);
}

int main()
{
	//MEASURETIME(TestStackAlloc());
	//
	//MEASURETIME(TestHeapAlloc());
	//
	//MEASURETIME(TestStdArray());
	//
	//MEASURETIME(TestStdVector());

	{
		Timer timer;

		this_thread::sleep_for(1s);
	}

	cin.get();
}

#endif // TEST02