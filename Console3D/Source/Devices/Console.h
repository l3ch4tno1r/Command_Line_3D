#pragma once

#define NOMINMAX
#include <Windows.h>

#include <string>
#include <thread>

#include "Geometry\Geometry3D\Transform3D.h"
#include "Geometry\Geometry2D\Transform2D.h"
#include "Geometry\Geometry3D\HVector3D.h"
#include "Geometry\Geometry2D\HVector2D.h"

#ifndef UNICODE
#error Please enable UNICODE for your compiler ! VS : \
Project Properties -> General -> Character Set -> Use Unicode.
#endif

#define TEST_CONSOLE 0

#if TEST_CONSOLE
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std::literals::chrono_literals;
#endif // TEST_CONSOLE

enum COLOUR
{
	FG_BLACK        = 0x0000,
	FG_DARK_BLUE    = 0x0001,
	FG_DARK_GREEN   = 0x0002,
	FG_DARK_CYAN    = 0x0003,
	FG_DARK_RED     = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW  = 0x0006,
	FG_GREY         = 0x0007,
	FG_DARK_GREY    = 0x0008,
	FG_BLUE         = 0x0009,
	FG_GREEN        = 0x000A,
	FG_CYAN         = 0x000B,
	FG_RED          = 0x000C,
	FG_MAGENTA      = 0x000D,
	FG_YELLOW       = 0x000E,
	FG_WHITE        = 0x000F,
	BG_BLACK        = 0x0000,
	BG_DARK_BLUE    = 0x0010,
	BG_DARK_GREEN   = 0x0020,
	BG_DARK_CYAN    = 0x0030,
	BG_DARK_RED     = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW  = 0x0060,
	BG_GREY         = 0x0070,
	BG_DARK_GREY    = 0x0080,
	BG_BLUE         = 0x0090,
	BG_GREEN        = 0x00A0,
	BG_CYAN         = 0x00B0,
	BG_RED          = 0x00C0,
	BG_MAGENTA      = 0x00D0,
	BG_YELLOW       = 0x00E0,
	BG_WHITE        = 0x00F0,
};


using namespace LCNMath::Geometry::Dim2;
using namespace LCNMath::Geometry::Dim3;

using Transform3Df = Transform3D<float>;
using Transform2Df = Transform2D<float>;
using HVector3Df   = HVector3D<float>;
using HVector2Df   = HVector2D<float>;

class Console
{
private:
	const float cm_ScreenDist = 0.1f;

private:
	//static Console console;

	UINT32 m_Width;
	UINT32 m_Height;

	CHAR_INFO* m_ScreenBuffer;
	HANDLE     m_HConsole;
	SMALL_RECT m_RectWindow;

	std::thread m_MainThread;

	float        m_Focal;
	Transform3Df m_R0ToCam;

	Console();

	~Console();

	Console(const Console&) = delete;
	Console& operator=(const Console&) = delete;

	void ConstructConsole(size_t width, size_t height, size_t fontw, size_t fonth);

	void MainThread();

	using Pixel = HVector2D<int>;

	struct Triangle2D
	{
		union
		{
			struct
			{
				Pixel p1;
				Pixel p2;
				Pixel p3;
			};
			Pixel pixels[3];
		};
	};

public:
	static Console& Get() noexcept;

	void Start();

	inline Transform3Df& R0ToCam()
	{
		return m_R0ToCam;
	}

	inline float& Focal()
	{
		return m_Focal;
	}

	inline UINT32 Width() const
	{
		return m_Width;
	}

	inline UINT32 Height() const
	{
		return m_Height;
	}

#if TEST_CONSOLE

#define WAIT if(!Wait()) return
#define RENDER_AND_WAIT Render(); WAIT

private:
	std::mutex              m_PauseMut;
	std::condition_variable m_PauseCondition;
	bool                    m_PauseNotified;
	bool                    m_Run;

	bool Wait();

public:
	void Notify(bool run);

#endif // TEST_CONSOLE

private:
	void Clear();

	// TODO : Include this in a collision detection API ?
	static HVector3Df SegmentPlaneIntersection(const HVector3Df& v1, const HVector3Df& v2, // Segment info
		                                       const HVector3Df& n,  const HVector3Df& p); // Plane info

	// TODO : Improve that !!!
	static uint ClipEdge(const HVector3Df& v1, const HVector3Df& v2, // Edge
		                 const HVector3Df& n,  const HVector3Df& p,  // Plane parameters
		                       HVector3Df& o1,       HVector3Df& o2);

	struct Triangle
	{
		HVector3Df vertices[3];
	};

	static uint ClipTriangle(const Triangle&   in_t,                         // Triangle
		                     const HVector3Df& n,    const HVector3Df& p,    // Plane parameters
		                           Triangle&   o_t1,       Triangle&  o_t2); // Output triangles

	char GetPixelValue(int x, int y) const;

	void DrawPoint(int x, int y, short c = 0x2588, short col = 0x000F);

	void DrawLine(int x1, int y1, int x2, int y2, short c = 0, short color = COLOUR::BG_WHITE);

	void DrawLine(const HVector2Df& v1, const HVector2Df& v2, char c = '#');

	void FillRectangle(const Pixel& TL, const Pixel& BR, char c = '#', const std::function<bool(const Pixel&)>& criteria = [](const Pixel&) { return true; });

	void FillTriangle(const HVector2Df& v1, const HVector2Df& v2, const HVector2Df& v3, char c = '#');

	void FillTriangle(const Triangle2D& triangle, char c = '#');

	struct ROI
	{
		Pixel TL;
		Pixel BR;

		int Width() const;
		int Height() const;

		Pixel TR() const;
		Pixel BL() const;
	};

	void FillTriangleRecursive(const Triangle2D& triangle, const ROI& aabb, char c = '#');

	enum Slots
	{
		_1 = 5,
		_2 = 4,
		_3 = 3,
		_4 = 2,
		_5 = 1,
	};

	void DisplayMessage(const std::string& msg, Slots slot);

	void HeartBeat();

	void Render();
};