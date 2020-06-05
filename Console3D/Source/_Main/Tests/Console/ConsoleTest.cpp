#include "Devices/Console.h"

#if TEST_CONSOLE

#include "Utilities/ErrorHandling.h"
#include "Utilities/Utils.h"
#include "Utilities/Instrumentor.h"

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

void Console::Notify(bool run)
{
	std::lock_guard<std::mutex> lock(m_PauseMut);

	m_Run = run;
	m_PauseNotified = true;

	m_PauseCondition.notify_one();
}

void Console::MainThread()
{
	Instrumentor::Get().BeginSession("Test fill triangle");

	m_PauseNotified = false;

	const int scale = 13;

	Pixel p1 = { scale * 0,  scale * 6 };
	Pixel p2 = { scale * 7,  scale * 0 };
	Pixel p3 = { scale * 10, scale * 8 };

	Pixel TL = {
		std::min({ p1.x, p2.x, p3.x }),
		std::min({ p1.y, p2.y, p3.y })
	};

	Pixel BR = {
		std::max({ p1.x, p2.x, p3.x }) + 1,
		std::max({ p1.y, p2.y, p3.y }) + 1
	};

	Clear();

	/*
	*/
	{
		PROFILE_SCOPE("Fill area");

		Triangle2D triangle = { p1, p2, p3 };

		FillTriangle(triangle, '#');
	}

	Clear();

	{
		PROFILE_SCOPE("Fill area old");

		Triangle2D triangle = { p1, p2, p3 };

		HVector2Df p1 = { (float)triangle.p1.x, (float)triangle.p1.y };
		HVector2Df p2 = { (float)triangle.p2.x, (float)triangle.p2.y };
		HVector2Df p3 = { (float)triangle.p3.x, (float)triangle.p3.y };

		FillTriangle(p1, p2, p3);
	}

	RENDER_AND_WAIT;

	Clear();

	DrawLine(BR.x, 0, BR.x, BR.y, '|');
	DrawLine(0, BR.y, BR.x, BR.y, '-');
	DrawPoint(BR.x, BR.y, '+');

	//RENDER_AND_WAIT;

	{
		PROFILE_SCOPE("Fill area recursive");

		FillTriangleRecursive({ p1, p2, p3 }, { TL, BR });
	}
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
