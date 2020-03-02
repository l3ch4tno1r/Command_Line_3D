#ifdef TEST07

#include <iostream>

class Vector3D
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};

		struct
		{
			float tab[3];
		};
	};
};

Vector3D operator+(const Vector3D& a, const Vector3D& b)
{
	static Vector3D result;

	for (uint8_t i = 0; i < 3; i++)
		result.tab[i] = a.tab[i] + b.tab[i];

	return result;
}

int main()
{
	Vector3D vec1 = { 1, 2, 3 };
	Vector3D vec2 = { 4, 5, 6 };

	Vector3D vec3 = vec1 + vec2;

	for (uint8_t i = 0; i < 3; i++)
		std::cout << vec3.tab[i] << std::endl;

	std::cin.get();
}

#endif // TEST07
