#ifdef TEST02

#include <iostream>
#include <vector>
#include <array>
#include <thread>

using namespace std;

#include "Stack\SMatrix.h"

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

#pragma region Alloc_Time_Comparison
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
#pragma endregion

using Mat_2_1_f = LCNMath::SMatrix<float, 2, 1>;

union HVector2D
{
	struct
	{
		float x;
		float y;
	};

	Mat_2_1_f mat;

	HVector2D() :
		mat(10.0f)
	{}
};

ostream& operator<<(ostream& stream, const HVector2D& vec)
{
	stream << "(" << vec.x << ", " << vec.y << ")";

	return stream;
}

union uTest
{
	struct
	{
		char c;
		double d;
		int i;
		char c2[3];
	};

	char tab[25];
};

int main()
{
	cout << sizeof(uTest) << endl;

	uTest test;

	test.tab[24] = 0;

	test.c = 'a';
	test.i = 2;

	cout << test.tab << endl;

	cin.get();
}

#endif // TEST02