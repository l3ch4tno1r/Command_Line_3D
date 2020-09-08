#include "Devices/Console.h"

#if TEST_CONSOLE

#include "Source/ErrorHandling.h"
#include "Source/Utils.h"
#include "Source/Instrumentor.h"

#include "Utilities/Angles.h"
#include "External/stb_image/stb_image.h"

#include "Geometry/Geometry2D/Transform2D.h"
#include "Geometry/Geometry2D/HVector2D.h"

#include "Graphics/Texture.h"

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

	const float scale = 2;

	Pixelf p1 = {  25.0f * scale, -22.0f * scale }; Pixelf t1 = { 1.0f, 0.0f }; // TL corner
	Pixelf p2 = { -25.0f * scale,  22.0f * scale }; Pixelf t2 = { 0.0f, 1.0f }; // TR corner
	Pixelf p3 = { -25.0f * scale, -22.0f * scale }; Pixelf t3 = { 0.0f, 0.0f }; // BL corner
	Pixelf p4 = {  25.0f * scale,  22.0f * scale }; Pixelf t4 = { 1.0f, 1.0f }; // BR corner

	Pixelf vt31 = t1 - t3;
	Pixelf vt32 = t2 - t3;
	Pixelf vt41 = t1 - t4;
	Pixelf vt42 = t2 - t4;

	Transform2Df textransform1 = {
		vt31.x, vt32.x, t3.x,
		vt31.y, vt32.y, t3.y,
		  0.0f,   0.0f, 1.0f
	};

	Transform2Df textransform2 = {
		vt41.x, vt42.x, t4.x,
		vt41.y, vt42.y, t4.y,
		  0.0f,   0.0f, 1.0f
	};

	// stb_image test
	size_t grayscalesize = 10;
	const char* grayscale = " .:-=+*#%@";

	std::string filepath("Ressource/Le_Chat_Noir_Photo_Medium.png");

	Texture texture;

	texture.Load("Ressource/Le_Chat_Noir_Photo_Medium.png", STBI_grey);

	if (!texture)
		return;

	Transform2Df tr1;
	Transform2Df tr2;

	auto mapper = [grayscale, grayscalesize](int i, int j,
		                                     const Transform2Df& tr,
		                                     const Transform2Df& textransform,
										     const Texture& texture)
	{
		Pixelf temp = textransform.mat * tr.mat * Pixelf((float)i, (float)j).mat;

		size_t tx = (texture.Width()  - 1) * std::min(std::max(temp.x, 0.0f), 1.0f);
		size_t ty = (texture.Height() - 1) * std::min(std::max(temp.y, 0.0f), 1.0f);

		ASSERT(tx < texture.Width());
		ASSERT(ty < texture.Height());

		unsigned char pxl = ((grayscalesize - 1) * texture(tx, ty)) / 255;

		ASSERT(pxl < grayscalesize);

		CHAR_INFO c;

		c.Char.UnicodeChar = grayscale[pxl];
		//c.Attributes = (0 == (i + j) % 2 ? COLOUR::BG_GREEN : COLOUR::BG_RED );
		c.Attributes = COLOUR::FG_WHITE | COLOUR::BG_BLACK;

		return c;
	};

	while (Continue())
	{
		Clear();

		Pixelf pix1 = frame.mat * p1.mat;
		Pixelf pix2 = frame.mat * p2.mat;
		Pixelf pix3 = frame.mat * p3.mat;
		Pixelf pix4 = frame.mat * p4.mat;

		Pixelf v31 = pix1 - pix3;
		Pixelf v32 = pix2 - pix3;
		Pixelf v41 = pix1 - pix4;
		Pixelf v42 = pix2 - pix4;

		tr1 = Transform2Df({
			v31.x, v32.x, pix3.x,
			v31.y, v32.y, pix3.y,
			0.0f, 0.0f, 1.0f
		}).mat.Invert();

		tr2 = Transform2Df({
			v41.x, v42.x, pix4.x,
			v41.y, v42.y, pix4.y,
			0.0f, 0.0f, 1.0f
			}).mat.Invert();

		namespace ph = std::placeholders;

		std::function<CHAR_INFO(int, int)> map = std::bind(mapper, ph::_1, ph::_2, std::cref(tr1), std::cref(textransform1), std::cref(texture));

		FillTriangleOLC(pix1.x, pix1.y, pix2.x, pix2.y, pix3.x, pix3.y, map);

		map = std::bind(mapper, ph::_1, ph::_2, std::cref(tr2), std::cref(textransform2), std::cref(texture));

		FillTriangleOLC(pix1.x, pix1.y, pix2.x, pix2.y, pix4.x, pix4.y, map);

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
