#include <iostream>

struct Vector3D
{
	union
	{
		struct
		{
			float x, y, z;
		};

		float mat[3];
	};

	Vector3D(float x, float y, float z) :
		x(x),
		y(y),
		z(z)
	{
		std::cout << "Construct" << std::endl;
	}

	Vector3D(const Vector3D& vec) :
		x(vec.x),
		y(vec.y),
		z(vec.z)
	{
		std::cout << "Copy" << std::endl;
	}

	inline float& operator[](unsigned int i)
	{
		return mat[i];
	}

	inline const float& operator[](unsigned int i) const
	{
		return mat[i];
	}
};

class Vector3D_Add
{
private:
	const Vector3D& a;
	const Vector3D& b;

public:
	Vector3D_Add(const Vector3D& _a, const Vector3D& _b) :
		a(_a),
		b(_b)
	{}

	operator Vector3D() const
	{
		return {
			(*this)[0],
			(*this)[1],
			(*this)[2]
		};
	}

	inline float operator[](unsigned int i) const
	{
		return a[i] + b[i];
	}

	float x() const
	{
		return (*this)[0];
	}

	float y() const
	{
		return (*this)[1];
	}

	float z() const
	{
		return (*this)[2];
	}
};

Vector3D_Add operator+(const Vector3D& a, const Vector3D& b)
{
	return Vector3D_Add(a, b);
}

/*
Vector3D operator+(const Vector3D& a, const Vector3D& b)
{
	return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
}
*/

int main()
{
	Vector3D a = { 1, 2, 3 };
	Vector3D b = { 4, 5, 6 };
	Vector3D c = { 7, 8, 9 };

	std::cout << "--------------------" << std::endl;

	Vector3D d = a + b + c;

	std::cout << "--------------------" << std::endl;

	std::cout << (a + b)[0] << ' ' << (a + b).x() << std::endl;
	std::cout << (a + b)[1] << ' ' << (a + b).y() << std::endl;
	std::cout << (a + b)[2] << ' ' << (a + b).z() << std::endl;

	std::cin.get();
}