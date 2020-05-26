#include "Devices/Console.h"

#ifdef TEST_CONSOLE

#include "Utilities/ErrorHandling.h"
#include "Utilities/Utils.h"

#include <iostream>
#include <sstream>

bool Console::Wait()
{
	std::unique_lock<std::mutex> lock(m_PauseMut);

	while (!m_PauseNotified)
		m_PauseCondition.wait(lock);

	m_PauseNotified = false;

	return m_Run;
}

void Console::FillRectangle(uint32_t tlx, uint32_t tly, uint32_t brx, uint32_t bry, char c)
{
	ASSERT(tlx <= brx);
	ASSERT(tly <= bry);

	for (uint32_t i = tlx; i < brx; ++i)
		for (uint32_t j = tly; j < bry; ++j)
			DrawPoint(i, j, c);
}

void Console::FillTriangleRecursive(const Triangle2D& triangle, const AABB2D& aabb, char c)
{
	// Temporary
	//FillRectangle(aabb.TL.x, aabb.TL.y, aabb.BR.x, aabb.BR.y, c);
	//++c;

	auto LineAABBInter = [](const Pixel& p1, const Pixel& p2, const AABB2D& aabb)
	{
		const Pixel offset = { 1, 1 };

		Pixel p1X2 = 2 * p1 + offset;
		Pixel p2X2 = 2 * p2 + offset;

		Pixel n = (p2X2 - p1X2).NormalVector();

		Pixel TL = 2 * aabb.TL;
		Pixel TR = { 2 * (aabb.BR.x + 1), 2 * aabb.TL.y };
		Pixel BL = { 2 * aabb.TL.x, 2 * (aabb.BR.y + 1) };
		Pixel BR = 2 * (aabb.BR + offset);

		short stl = sign((TL - p1X2) | n);
		short str = sign((TR - p1X2) | n);
		short sbl = sign((BL - p1X2) | n);
		short sbr = sign((BR - p1X2) | n);

		return (stl + str + sbl + sbr) / 4;
	};

	auto InsideTriangle = [](const Triangle2D& triangle, const Pixel& pixel)
	{
		Pixel offset = { 1, 1 };

		Triangle2D triangleX2 = {
			2 * triangle.p1 + offset,
			2 * triangle.p2 + offset,
			2 * triangle.p3 + offset
		};

		Pixel pixelX2 = 2 * pixel + offset;

		Pixel n1 = (triangleX2.p2 - triangleX2.p1).NormalVector();
		Pixel n2 = (triangleX2.p3 - triangleX2.p2).NormalVector();
		Pixel n3 = (triangleX2.p1 - triangleX2.p3).NormalVector();

		short s1 = sign((pixelX2 - triangleX2.p1) | n1);
		short s2 = sign((pixelX2 - triangleX2.p2) | n2);
		short s3 = sign((pixelX2 - triangleX2.p3) | n3);

		return (s1 == s2) && (s2 == s3);
	};

	auto InsideAABB = [](const Pixel& p, const AABB2D& aabb)
	{
		const Pixel offset = { 1, 1 };

		Pixel pX2 = 2 * p + offset;

		Pixel TL = 2 * aabb.TL;
		Pixel BR = 2 * (aabb.BR + offset);

		bool a = (pX2.x >= TL.x) && (pX2.x <= BR.x);
		bool b = (pX2.y >= TL.y) && (pX2.y <= BR.y);

		return a && b;
	};

	int A, B, C;

	A = LineAABBInter(triangle.p1, triangle.p2, aabb);
	B = LineAABBInter(triangle.p2, triangle.p3, aabb);
	C = LineAABBInter(triangle.p3, triangle.p1, aabb);

	if (A < 0 || B < 0 || c < 0)
		return;

	struct Flags
	{
		union
		{
			struct
			{
				char a : 1;
				char b : 1;
				char c : 1;
			};
			char total;
		};
	} flags;

	flags.a = A > 0;
	flags.b = B > 0;
	flags.c = C > 0;

	uint8_t count = 0;

	for (uint8_t i = 1; i < 8; i = i << 1)
		if (flags.total & i)
			++count;

	std::stringstream sstr;

	sstr << "Case : " << (int)count;

	DisplayMessage(sstr.str(), Slots::_5);

	switch (count)
	{
	case 0:
	case 2:
	{
		//BREACKIF(aabb.BR.x - aabb.TL.x == 2 || aabb.BR.y - aabb.TL.y == 2);

		if ((aabb.BR.x - aabb.TL.x <= 1) && (aabb.BR.y - aabb.TL.y <= 1))
		{
			if (InsideTriangle(triangle, aabb.TL))
				DrawPoint(aabb.TL.x, aabb.TL.y, c);

			return;
		}

		int midx = (aabb.TL.x + aabb.BR.x) / 2;
		int midy = (aabb.TL.y + aabb.BR.y) / 2;

		AABB2D subaabbs[] = {
			{ { aabb.TL.x, aabb.TL.y }, { midx,      midy      } },
			{ { midx,      aabb.TL.y }, { aabb.BR.x, midy      } },
			{ { aabb.TL.x, midy      }, { midx,      aabb.BR.y } },
			{ { midx,      midy      }, { aabb.BR.x, aabb.BR.y } }
		};

		for (const AABB2D& _aabb : subaabbs)
			FillTriangleRecursive(triangle, _aabb, c);

		break;
	}
	case 1:
	{
		if ((aabb.BR.x - aabb.TL.x <= 1) && (aabb.BR.y - aabb.TL.y <= 1))
		{
			DrawPoint(aabb.BR.x, aabb.BR.y, c);

			return;
		}

		bool inter = false;

		for (const Pixel& pixel : triangle.pixels)
			if (InsideAABB(pixel, aabb))
			{
				inter = true;
				break;
			}

		if (inter)
		{
			int midx = (aabb.TL.x + aabb.BR.x) / 2;
			int midy = (aabb.TL.y + aabb.BR.y) / 2;

			AABB2D subaabbs[] = {
				{ { aabb.TL.x, aabb.TL.y }, { midx,      midy      } },
				{ { midx,      aabb.TL.y }, { aabb.BR.x, midy      } },
				{ { aabb.TL.x, midy      }, { midx,      aabb.BR.y } },
				{ { midx,      midy      }, { aabb.BR.x, aabb.BR.y } }
			};

			for (const AABB2D& _aabb : subaabbs)
				FillTriangleRecursive(triangle, _aabb, c);
		}

		break;
	}
	case 3:
	{
		FillRectangle(aabb.TL.x, aabb.TL.y, aabb.BR.x, aabb.BR.y, c);

		break;
	}
	default:
		break;
	}

	RENDER_AND_WAIT;
}

void Console::Notify(bool run)
{
	std::lock_guard<std::mutex> lock(m_PauseMut);

	m_Run = run;
	m_PauseNotified = true;

	m_PauseCondition.notify_one();
}

void Console::MainThread()
{
	m_PauseNotified = false;

	Pixel p1 = { 0,  6 };
	Pixel p2 = { 7,  0 };
	Pixel p3 = { 10, 8 };

	Pixel TL = {
		std::min({ p1.x, p2.x, p3.x }),
		std::min({ p1.y, p2.y, p3.y })
	};

	Pixel BR = {
		std::max({ p1.x, p2.x, p3.x }) + 1,
		std::max({ p1.y, p2.y, p3.y }) + 1
	};

	Clear();

	DrawLine(BR.x, 0, BR.x, BR.y, '|');
	DrawLine(0, BR.y, BR.x, BR.y, '-');
	DrawPoint(BR.x, BR.y, '+');

	RENDER_AND_WAIT;

	FillTriangleRecursive({ p1, p2, p3 }, { TL, BR }, 'a');
	/*
	FillRectangle(TL.x, TL.y, BR.x, BR.y);
	*/

	DisplayMessage(std::string("Done !"), Slots::_3);
	RENDER_AND_WAIT;

	/*
	TL = { 0, 0 };
	BR = { 5, 4 };

	FillTriangleRecursive({ p1, p2, p3 }, { TL, BR });
	RENDER_AND_WAIT;

	TL = { 5, 4 };
	BR = { 7, 5 };

	FillTriangleRecursive({ p1, p2, p3 }, { TL, BR });
	RENDER_AND_WAIT;

	TL = { 0, 0 };
	BR = { 1, 1 };

	FillTriangleRecursive({ p1, p2, p3 }, { TL, BR });
	RENDER_AND_WAIT;
	*/
}
#endif
