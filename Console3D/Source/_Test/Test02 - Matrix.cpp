#ifdef TEST02

#include <iostream>
#include <vector>
#include <array>

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

void StackAlloc()
{
	for (int i = 0; i < 1000000; i++)
		array<int, 10>();
}

void HeapAlloc()
{
	for (int i = 0; i < 1000000; i++)
		vector<int>(10);
}

int main()
{
	MEASURETIME(StackAlloc());

	MEASURETIME(HeapAlloc());

	cin.get();
}

#endif // TEST02