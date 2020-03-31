#ifdef TEST07

#include <iostream>

class HVector3D
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

HVector3D operator+(const HVector3D& a, const HVector3D& b)
{
	static HVector3D result;

	for (uint8_t i = 0; i < 3; i++)
		result.tab[i] = a.tab[i] + b.tab[i];

	return result;
}

int main()
{
	HVector3D vec1 = { 1, 2, 3 };
	HVector3D vec2 = { 4, 5, 6 };

	HVector3D vec3 = vec1 + vec2;

	for (uint8_t i = 0; i < 3; i++)
		std::cout << vec3.tab[i] << std::endl;

	std::cin.get();
}

#endif // TEST07
