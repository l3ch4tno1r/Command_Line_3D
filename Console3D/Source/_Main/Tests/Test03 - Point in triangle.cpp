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

int main()
{
	HVector2D v1(4, 15);
	HVector2D v2(15, 18);
	HVector2D v3(9, 6);

	auto insidetriangle = [&](const HVector2D& p)
	{
		HVector2D v1v2 = v2 - v1; HVector2D n1 = { -v1v2.y, v1v2.x };
		HVector2D v2v3 = v3 - v2; HVector2D n2 = { -v2v3.y, v2v3.x };
		HVector2D v3v1 = v1 - v3; HVector2D n3 = { -v3v1.y, v3v1.x };

		short s1 = sign((p - v1) | n1);
		short s2 = sign((p - v2) | n2);
		short s3 = sign((p - v3) | n3);

		return s1 == s2 && s2 == s3;
	};

	char img[20][20];

	memset(img, 0, 20 * 20);

	img[(uint32_t)v1.x][(uint32_t)v1.y] = '#';
	img[(uint32_t)v2.x][(uint32_t)v2.y] = '#';
	img[(uint32_t)v3.x][(uint32_t)v3.y] = '#';

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
			HVector2D(x,     y + 1),
			HVector2D(x - 1, y),
			HVector2D(x,     y - 1)
		};

		for (const HVector2D& pt : voisins)
		{
			if (img[(uint32_t)pt.x][(uint32_t)pt.y] == 0)
			{
				if (insidetriangle(pt))
				{
					img[(uint32_t)pt.x][(uint32_t)pt.y] = '#';
					queue.push(pt);
				}
				else
				{
					HVector2D temp[] = {
						HVector2D(pt.x + 1, pt.y),
						HVector2D(pt.x,     pt.y + 1),
						HVector2D(pt.x - 1, pt.y),
						HVector2D(pt.x,     pt.y - 1)
					};

					for (const HVector2D& p : temp)
					{
						if (insidetriangle(p))
						{
							img[(uint32_t)pt.x][(uint32_t)pt.y] = '#';
							queue.push(pt);
							break;
						}
					}
				}
			}
		}
	}

	display();

	std::cin.get();
}