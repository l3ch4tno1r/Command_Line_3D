#include "Devices/Console.h"

#if TEST_CONSOLE

#include "Source/ErrorHandling.h"
#include "Source/Utils.h"
#include "Source/Instrumentor.h"

#include "Utilities/Angles.h"
#include "Geometry/Geometry2D/Transform2D.h"
#include "Geometry/Geometry2D/HVector2D.h"

#include <iostream>
#include <sstream>

using namespace LCNMath::Geometry::Dim2;

using Pixelf = HVector2Df;

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

#if false
void Console::MainThread()
{
	Instrumentor::Get().BeginSession("Test fill triangle");

	m_PauseNotified = false;

	const int scale = 9;

	Pixel p1 = { scale * 0,  scale * 6 };
	Pixel p2 = { scale * 7,  scale * 0 };
	Pixel p3 = { scale * 10, scale * 8 };

	this->Clear();

	/*
	{
		PROFILE_SCOPE("Fill area old");

		Triangle2D triangle = { p1, p2, p3 };

		HVector2Df p1 = { (float)triangle.p1.x, (float)triangle.p1.y };
		HVector2Df p2 = { (float)triangle.p2.x, (float)triangle.p2.y };
		HVector2Df p3 = { (float)triangle.p3.x, (float)triangle.p3.y };

		FillTriangle(p1, p2, p3);
	}
	*/

	{
		PROFILE_SCOPE("Fill area");

		this->DrawLine(p1.x, p1.y, p2.x, p2.y, 0, COLOUR::BG_RED);
		this->DrawLine(p2.x, p2.y, p3.x, p3.y, 0, COLOUR::BG_RED);
		this->DrawLine(p3.x, p3.y, p1.x, p1.y, 0, COLOUR::BG_RED);

		this->FillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
	}
	
	this->DisplayMessage(std::string("Done !"), Slots::_3);
	RENDER_AND_WAIT;
}
#endif

#if true

void Console::FillTriangleOLC(int x1, int y1, int x2, int y2, int x3, int y3, const MapFunction& mapper)
{
	// From OneLoneCoder : https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h

	auto SWAP = [](int &x, int &y) { int t = x; x = y; y = t; };
	auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) DrawPoint(i, ny, mapper); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
	if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
	if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
									 // Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
	}
}

void Console::MainThread()
{
	float theta  = 0;
	float aspeed = 6;            // degrès secondes
	float dt     = 2.0f * 16.0f / 1000; // 16ms

	Transform2Df frame = {
		1.0f, 0.0f, (float)m_Width / 2,
		0.0f, 1.0f, (float)m_Height / 2,
		0.0f, 0.0f, 1.0f
	};

	Pixelf p1 = {  0.0f,  -30.0f };
	Pixelf p2 = {  30.0f,  10.0f };
	Pixelf p3 = { -20.0f,  0.0f };

	while (Continue())
	{
		Clear();

		Pixelf pix1 = frame.mat * p1.mat;
		Pixelf pix2 = frame.mat * p2.mat;
		Pixelf pix3 = frame.mat * p3.mat;

		Pixelf TL = { std::min({ pix1.x, pix2.x, pix3.x }), std::min({ pix1.y, pix2.y, pix3.y }) };
		Pixelf BR = { std::max({ pix1.x, pix2.x, pix3.x }), std::max({ pix1.y, pix2.y, pix3.y }) };
		Pixelf BL = { TL.x, BR.y };
		Pixelf TR = { BR.x, TL.y };

		auto mapper = [](int i, int j)
		{
			CHAR_INFO c;

			c.Char.UnicodeChar = 0;
			c.Attributes = (0 == (i + j) % 2 ? COLOUR::BG_GREEN : COLOUR::BG_RED );

			return c;
		};

		FillTriangleOLC(pix1.x, pix1.y, pix2.x, pix2.y, pix3.x, pix3.y, mapper);

		/*
		DrawLine(pix1.x, pix1.y, pix2.x, pix2.y, 0, COLOUR::BG_GREEN);
		DrawLine(pix2.x, pix2.y, pix3.x, pix3.y, 0, COLOUR::BG_GREEN);
		DrawLine(pix3.x, pix3.y, pix1.x, pix1.y, 0, COLOUR::BG_GREEN);

		DrawLine(TL.x, TL.y, TR.x, TR.y, 0, COLOUR::BG_RED);
		DrawLine(TR.x, TR.y, BR.x, BR.y, 0, COLOUR::BG_RED);
		DrawLine(BR.x, BR.y, BL.x, BL.y, 0, COLOUR::BG_RED);
		DrawLine(BL.x, BL.y, TL.x, TL.y, 0, COLOUR::BG_RED);

		Pixelf pix = frame.mat * p1.mat;

		DrawLine(frame.Tx, frame.Ty, pix.x, pix.y, 0, COLOUR::BG_RED);
		*/

		theta += aspeed * dt;

		frame.mat(0, 0) =  std::cos(TORAD(theta));
		frame.mat(0, 1) = -std::sin(TORAD(theta));
		frame.mat(1, 0) =  std::sin(TORAD(theta));
		frame.mat(1, 1) =  std::cos(TORAD(theta));

		Render();
	}
}
#endif
#endif
