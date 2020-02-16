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

union Vector2D
{
	struct
	{
		float x;
		float y;
	};

	Matrix::SMatrix<float, 2, 1> mat;
};

ostream& operator<<(ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.mat(0, 0) << ", " << vec.mat(1, 0) << ")";

	return stream;
}

int main()
{
	Vector2D vec = { 1.0, 2.0 };

	cout << vec << endl;

	cin.get();
}

#endif // TEST02