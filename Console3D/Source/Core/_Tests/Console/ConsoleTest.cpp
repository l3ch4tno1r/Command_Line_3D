#if TEST_CONSOLE
#include "Devices/Console.h"

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

	Texture texture;

	texture.Load("Ressource/Le_Chat_Noir_Photo_Medium.png", STBI_grey);
	/*
	texture.Load("Ressource/Wooden_Medium.jpg", STBI_grey);
	*/

	if (!texture)
		return;

	Transform2Df tr1;
	Transform2Df tr2;

	auto mapper = [](int i, int j,
		             const Transform2Df& tr,
		             const Transform2Df& textransform,
	                 const Texture& texture)
	{
		static const size_t grayscalesize = 10;
		static const char* grayscale = " .:-=+*#%@";

		Pixelf temp = textransform.mat * tr.mat * Pixelf((float)i, (float)j).mat;

		size_t tx = (texture.Width()  - 1) * std::min(std::max(temp.x, 0.0f), 1.0f);
		size_t ty = (texture.Height() - 1) * std::min(std::max(temp.y, 0.0f), 1.0f);

		ASSERT(tx < texture.Width());
		ASSERT(ty < texture.Height());

		unsigned char pxl = ((grayscalesize - 1) * texture(tx, ty)) / 255;

		ASSERT(pxl < grayscalesize);

		CHAR_INFO c;

		c.Char.UnicodeChar = grayscale[pxl];
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

		std::function<CHAR_INFO(int, int)> map = std::bind(
			mapper,
			ph::_1,
			ph::_2,
			std::cref(tr1),
			std::cref(textransform1),
			std::cref(texture));

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
