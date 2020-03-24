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
		Vector3D result = {
			a.x + b.x,
			a.y + b.y,
			a.z + b.z
		};

		return result;
	}

	float operator[](unsigned int i)
	{
		return a[i] + b[i];
	}

	float x() const
	{
		return a.x + b.x;
	}

	float y() const
	{
		return a.y + b.y;
	}

	float z() const
	{
		return a.z + b.z;
	}
};

Vector3D_Add operator+(const Vector3D& a, const Vector3D& b)
{
	return Vector3D_Add(a, b);
}

int main()
{
	Vector3D a = { 1, 2, 3 };
	Vector3D b = { 4, 5, 6 };

	Vector3D c = a + b;

	std::cout << (a + b)[0] << ' ' << (a + b).x() << std::endl;
	std::cout << (a + b)[1] << ' ' << (a + b).y() << std::endl;
	std::cout << (a + b)[2] << ' ' << (a + b).z() << std::endl;

	std::cin.get();
}