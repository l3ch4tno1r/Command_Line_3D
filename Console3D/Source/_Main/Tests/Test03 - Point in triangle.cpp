#include <iostream>
#include <queue>
#include <stack>
#include <cmath>

#include "Geometry/Geometry2D/HVector2D.h"
#include "Utilities/Utils.h"
#include "Utilities/ErrorHandling.h"

using namespace LCNMath::Geometry::Dim2;

struct Pixel
{
	int x;
	int y;

	Pixel(int _x, int _y) :
		x(_x),
		y(_y)
	{}

	Pixel(const Pixel& vec) :
		x(vec.x),
		y(vec.y)
	{}

	Pixel& operator*=(int a)
	{
		x *= a;
		y *= a;

		return *this;
	}

	Pixel& operator+=(const Pixel& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}
};

Pixel operator+(const Pixel& p1, const Pixel& p2)
{
	return Pixel(p1.x + p2.x, p1.y + p2.y);
}

Pixel operator-(const Pixel& p1, const Pixel& p2)
{
	return Pixel(p1.x - p2.x, p1.y - p2.y);
}

Pixel operator*(int a, const Pixel& p1)
{
	return Pixel(a * p1.x, a * p1.y);
}

Pixel operator/(const Pixel& p1, int a)
{
	return Pixel(p1.x / a, p1.y / a);
}

int operator|(const Pixel& p1, const Pixel& p2)
{
	return p1.x * p2.x + p1.y * p2.y;
}

Pixel FromHVector2D(const HVector2D& other)
{
	return {
		(int)std::floor(other.x),
		(int)std::floor(other.y)
	};
}

int main()
{
	HVector2D v1(4,  17);
	HVector2D v2(15, 17);
	HVector2D v3(9,  6);

	auto insidetriangle = [&](const HVector2D& p)
	{
		Pixel p1 = FromHVector2D(v1); (p1 *= 2) += Pixel(1, 1);
		Pixel p2 = FromHVector2D(v2); (p2 *= 2) += Pixel(1, 1);
		Pixel p3 = FromHVector2D(v3); (p3 *= 2) += Pixel(1, 1);

		Pixel _p = FromHVector2D(p); (_p *= 2) += Pixel(1, 1);

		Pixel p1p2 = p2 - p1; Pixel n1 = { -p1p2.y, p1p2.x };
		Pixel p2p3 = p3 - p2; Pixel n2 = { -p2p3.y, p2p3.x };
		Pixel p3p1 = p1 - p3; Pixel n3 = { -p3p1.y, p3p1.x };

		short s1 = sign((_p - p1) | n1); //ASSERT(s1 != 0);
		short s2 = sign((_p - p2) | n2); //ASSERT(s2 != 0);
		short s3 = sign((_p - p3) | n3); //ASSERT(s3 != 0);

		return (s1 == s2 && s2 == s3) || (s1 == 0 || s2 == 0 || s3 == 0);
	};

	char img[20][20];

	memset(img, 0, 20 * 20);

	img[(uint32_t)v1.x][(uint32_t)v1.y] = '+';
	img[(uint32_t)v2.x][(uint32_t)v2.y] = '+';
	img[(uint32_t)v3.x][(uint32_t)v3.y] = '+';

	auto display = [&]()
	{
		for (uint32_t i = 0; i < 20; i++)
		{
			for (uint32_t j = 0; j < 20; j++)
				std::cout << img[i][j];

			std::cout << std::endl;
		}
	};

	display();

	std::queue<HVector2D> queue;

	queue.push((v1 + v2 + v3) / 3);

	img[(uint32_t)queue.front().x][(uint32_t)queue.front().y] = '#';

	while (!queue.empty())
	{
		uint32_t x = queue.front().x;
		uint32_t y = queue.front().y;

		/*
		display();

		std::cin.get();
		*/

		queue.pop();

		HVector2D voisins[] = {
			HVector2D(x + 1, y),
			HVector2D(x + 1, y + 1),
			HVector2D(x,     y + 1),
			HVector2D(x - 1, y + 1),
			HVector2D(x - 1, y),
			HVector2D(x - 1, y - 1),
			HVector2D(x,     y - 1),
			HVector2D(x + 1, y - 1)
		};

		for (const HVector2D& pt : voisins)
		{
			if (img[(uint32_t)pt.x][(uint32_t)pt.y] != '#')
			{
				if (insidetriangle(pt))
				{
					img[(uint32_t)pt.x][(uint32_t)pt.y] = '#';
					queue.push(pt);
				}
			}
		}
	}

	display();

	std::cin.get();
}