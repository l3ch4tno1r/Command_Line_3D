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
	Pixel v1( 4, 18);
	Pixel v2(15, 18);
	Pixel v3( 9,  6);

	auto insidetriangle = [&](const Pixel& p)
	{
		Pixel v1v2 = v2 - v1; Pixel n1 = { -v1v2.y, v1v2.x };
		Pixel v2v3 = v3 - v2; Pixel n2 = { -v2v3.y, v2v3.x };
		Pixel v3v1 = v1 - v3; Pixel n3 = { -v3v1.y, v3v1.x };

		short s1 = sign((p - v1) | n1);
		short s2 = sign((p - v2) | n2);
		short s3 = sign((p - v3) | n3);

		return s1 == s2 && s2 == s3;
	};

	char img[20][20];

	memset(img, 0, 20 * 20);

	img[v1.x][v1.y] = '#';
	img[v2.x][v2.y] = '#';
	img[v3.x][v3.y] = '#';

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

	std::queue<Pixel> queue;

	queue.push((v1 + v2 + v3) / 3);

	img[queue.front().x][queue.front().y] = '#';

	int iterations = 0;

	while (!queue.empty())
	{
		uint32_t x = queue.front().x;
		uint32_t y = queue.front().y;

		/*
		display();

		std::cin.get();
		*/

		queue.pop();

		Pixel voisins[] = {
			Pixel(x + 1, y),
			Pixel(x,     y + 1),
			Pixel(x - 1, y),
			Pixel(x,     y - 1)
		};

		for (const Pixel& pt : voisins)
		{
			if (insidetriangle(pt) && (img[pt.x][pt.y] == 0))
			{
				img[pt.x][pt.y] = '#';
				queue.push(pt);
			}
		}

		++iterations;
	}

	display();

	std::cin.get();
}