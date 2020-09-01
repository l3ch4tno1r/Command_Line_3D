#include "Console.h"

#include "Utilities\ErrorHandling.h"
#include "Utilities\Angles.h"
#include "Utilities\TimeMeasurement.h"
#include "Utilities\Utils.h"
#include "Utilities\Instrumentor.h"

#include "Devices\PaceMaker.h"

#include "Graphics\OBJReader.h"

#include <chrono>
#include <cmath>
#include <sstream>
#include <array>
#include <algorithm>
#include <utility>
#include <list>
#include <queue>

/*
#define DRAW_FACES
*/
#define DRAW_EDGES

Console::Console() :
	m_Width(0),
	m_Height(0),
	m_ScreenBuffer(nullptr),
	m_HConsole(nullptr),
	m_Focal(120.0f)
	//m_Focal(90.0f)
{
	m_HConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set up Camera transform
	m_R0ToCam.Rux =   1.0f;
	m_R0ToCam.Ruy =   0.0f;
	m_R0ToCam.Ruz =   0.0f;
	m_R0ToCam.Rvx =   0.0f;
	m_R0ToCam.Rvy =   0.0f;
	m_R0ToCam.Rvz =   1.0f;
	m_R0ToCam.Rwx =   0.0f;
	m_R0ToCam.Rwy =  -1.0f;
	m_R0ToCam.Rwz =   0.0f;
	m_R0ToCam.Tx  =   0.0f;
	m_R0ToCam.Ty  =  10.0f;
	m_R0ToCam.Tz  =   1.8f;
}

Console::~Console()
{
	delete[] m_ScreenBuffer;
}

void Console::ConstructConsole(size_t width, size_t height, size_t fontw, size_t fonth)
{
	if (m_HConsole == INVALID_HANDLE_VALUE)
		throw std::exception("Handle invalid.");

	m_Width  = width;
	m_Height = height;

	m_RectWindow = { 0, 0, 1, 1 };

	SetConsoleWindowInfo(m_HConsole, TRUE, &m_RectWindow);

	COORD coord = { (short)m_Width, (short)m_Height };

	if (!SetConsoleScreenBufferSize(m_HConsole, coord))
		throw std::exception("Cannot set Console screen buffer size.");

	if (!SetConsoleActiveScreenBuffer(m_HConsole))
		throw std::exception("Cannot set Console active screen buffer.");

	m_RectWindow = { 0, 0, (short)m_Width - 1, (short)m_Height - 1 };

	CONSOLE_FONT_INFOEX cfi;

	cfi.cbSize       = sizeof(cfi);
	cfi.nFont        = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily   = FF_DONTCARE;
	cfi.FontWeight   = FW_NORMAL;

	wcscpy_s(cfi.FaceName, L"Consolas");

	if (!SetCurrentConsoleFontEx(m_HConsole, FALSE, &cfi))
		throw std::exception("Cannot set Console font.");

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(m_HConsole, &csbi))
		throw std::exception("Cannot get Console screen buffer info.");

	if (m_Width > csbi.dwMaximumWindowSize.X)
		throw std::exception("Screen Width / Font Width Too Big.");

	if (m_Height > csbi.dwMaximumWindowSize.Y)
		throw std::exception("Screen Height / Font Height Too Big.");

	// Set console size
	if (!SetConsoleWindowInfo(m_HConsole, TRUE, &m_RectWindow))
		throw std::exception("Cannot set Console physical size.");

	// Allocate buffer
	m_ScreenBuffer = new CHAR_INFO[m_Width * m_Height];
}

#if !TEST_CONSOLE
void Console::MainThread()
{
	Model3D models[] = {
		OBJReader().ReadFile<Model3D>("Ressource/null.obj", false),
		/*
		OBJReader().ReadFile<Model3D>("Ressource/carpet.obj", false),
		OBJReader().ReadFile<Model3D>("Ressource/debug.obj", true)
		OBJReader().ReadFile<Model3D>("Ressource/octogon_no_normals.obj", true)
		OBJReader().ReadFile<Model3D>("Ressource/triangle.obj", false)
		OBJReader().ReadFile<Model3D>("Ressource/cube.obj", false)
		OBJReader().ReadFile<Model3D>("Ressource/octogon.obj", false)
		OBJReader().ReadFile<Model3D>("Ressource/table_basique.obj", false)
		OBJReader().ReadFile<Model3D>("Ressource/teapot.obj", true)
		*/
		OBJReader().ReadFile<Model3D>("Ressource/axisr.obj", true)
	};

	// Quick fix for teapot
	/*
	Transform3Df teapot({
		1.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 1.0f,  0.0f, 0.0f,
		0.0f, 0.0f,  0.0f, 1.0f
	});

	for (HVector3Df& vertex : models[1].Vertices())
		vertex = teapot * vertex;

	for (HVector3Df& vertex : models[1].Normals())
		vertex = teapot * vertex;
	*/

	//const float scalefactor = 1.0f;
	const float scalefactor[] = { 1.0f, 1.0f };

	// Scaling models
	for (uint32_t i = 0; i < 2; ++i)
	{
		for (HVector3Df& v : models[i].Vertices())
		{
			v.x *= scalefactor[i];
			v.y *= scalefactor[i];
			v.z *= scalefactor[i];
		}
	}

	Transform3Df R0ToObjs[] = {
		Transform3Df(),
		Transform3Df()
	};

	const float tab[3][4] = {
		m_Focal, 0.0f,    0.0f,  0.0f,
		0.0f,    m_Focal, 0.0f,  0.0f,
		0.0f,    0.0f,    1.0f,  0.0f,
	};

	LCNMath::Matrix::StaticMatrix::Matrix<float, 3, 4> Projection(tab);

	Transform2Df ImgToCam(m_Width / 2, m_Height / 2, 180.0f);

	// For clipping

	std::pair<HVector3Df, HVector3Df> planes[] = {
		{ HVector3Df(HVector3Df::Z()),                        HVector3Df(0.0f, 0.0f, 0.5f) },
		{ HVector3Df( 0,       m_Focal, m_Height / 2, false), HVector3Df(0.0f, 0.0f, 0.0f) },
		{ HVector3Df( 0,      -m_Focal, m_Height / 2, false), HVector3Df(0.0f, 0.0f, 0.0f) },
		{ HVector3Df(-m_Focal, 0,       m_Width  / 2, false), HVector3Df(0.0f, 0.0f, 0.0f) },
		{ HVector3Df( m_Focal, 0,       m_Width  / 2, false), HVector3Df(0.0f, 0.0f, 0.0f) }
	};

	std::pair<HVector3Df, HVector3Df> planesfromObj[] = {
		{ HVector3Df(0.0f, 0.0f, 0.0f, false), HVector3Df(0.0f, 0.0f, 0.0f)},
		{ HVector3Df(0.0f, 0.0f, 0.0f, false), HVector3Df(0.0f, 0.0f, 0.0f)},
		{ HVector3Df(0.0f, 0.0f, 0.0f, false), HVector3Df(0.0f, 0.0f, 0.0f)},
		{ HVector3Df(0.0f, 0.0f, 0.0f, false), HVector3Df(0.0f, 0.0f, 0.0f)},
		{ HVector3Df(0.0f, 0.0f, 0.0f, false), HVector3Df(0.0f, 0.0f, 0.0f)}
	};

	LCNMath::Matrix::StaticMatrix::Matrix<float, 3, 4> _Proj = ImgToCam.mat * Projection;

	// FPS measurement
	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	// Ligthing
	HVector3Df light = { -1, -5, -1, false };
	light.Normalize();

	const char* grayscale = " .:-=+%*#@";

	// For info display
	std::stringstream sstr;

	// Console device loop
	while (Continue())
	{
		STARTCHRONO;

		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> ellapsedtime = tp2 - tp1;
		tp1 = tp2;

		Clear();

		Transform3Df CamToR0 = m_R0ToCam.mat.Invert();

		// Loop through models
		for (uint i = 0; i < 2; i++)
		{
			// TODO : Remettre a const
			Model3D& model = models[i];

			Transform3Df CamToObj = CamToR0 * R0ToObjs[i];
			Transform3Df ObjToCam = CamToObj.mat.Invert();

			for (uint i = 0; i < 5; i++)
			{
				planesfromObj[i].first  = ObjToCam * planes[i].first;
				planesfromObj[i].second = ObjToCam * planes[i].second;
			}

#ifdef DRAW_FACES
			/*
			std::sort(model.Faces().begin(), model.Faces().end(), [&](const Model3D::Face& face1, const Model3D::Face& face2)
			{
				const HVector3Df& f1v1 = model.Vertices()[face1.v1];
				const HVector3Df& f1v2 = model.Vertices()[face1.v2];
				const HVector3Df& f1v3 = model.Vertices()[face1.v3];

				const HVector3Df& f2v1 = model.Vertices()[face2.v1];
				const HVector3Df& f2v2 = model.Vertices()[face2.v2];
				const HVector3Df& f2v3 = model.Vertices()[face2.v3];

				HVector3Df ObjToCamPos = { ObjToCam.Tx, ObjToCam.Ty, ObjToCam.Tz };

				float z1 = ((f1v1 + f1v2 + f1v3) / 3 - ObjToCamPos | planesfromObj[0].first);
				float z2 = ((f2v1 + f2v2 + f2v3) / 3 - ObjToCamPos | planesfromObj[0].first);

				return z1 > z2;
			});
			*/

			for (const Model3D::Face& face : model.Faces())
			{
				Triangle triangle = {
					model.Vertices()[face.v1],
					model.Vertices()[face.v2],
					model.Vertices()[face.v3]
				};

				HVector3Df cam2v1 = triangle.vertices[0] - planesfromObj[1].second;
				
				const HVector3Df& nface = model.Normals()[face.vn1];
				
				if ((cam2v1 | nface) > 0.0f)
					continue;

				static Triangle o1 = {
					HVector3Df(0, 0, 0),
					HVector3Df(0, 0, 0),
					HVector3Df(0, 0, 0)
				};

				static Triangle o2 = {
					HVector3Df(0, 0, 0),
					HVector3Df(0, 0, 0),
					HVector3Df(0, 0, 0)
				};

				std::list<Triangle> clippedtriangles({ triangle });

				for (const auto& plane : planesfromObj)
				{
					auto it = clippedtriangles.begin();

					while (it != clippedtriangles.end())
					{
						uint num = ClipTriangle(*it, plane.first, plane.second, o1, o2);

						switch (num)
						{
						case 0:
							it = clippedtriangles.erase(it);

							break;
						case 1:
							clippedtriangles.push_front(o1);

							it = clippedtriangles.erase(it);

							break;
						case 2:
							clippedtriangles.push_front(o1);
							clippedtriangles.push_front(o2);

							it = clippedtriangles.erase(it);

							break;
						case 3:
						default:
							++it;

							break;
						}

						if (num == 0)
							break;
					}
				}

				for (const auto& t : clippedtriangles)
				{
					HVector2Df _pt1 = _Proj * (CamToObj * t.vertices[0]).mat;
					HVector2Df _pt2 = _Proj * (CamToObj * t.vertices[1]).mat;
					HVector2Df _pt3 = _Proj * (CamToObj * t.vertices[2]).mat;

					_pt1.Homogenize();
					_pt2.Homogenize();
					_pt3.Homogenize();

					HVector3Df _n = R0ToObjs[i] * nface;

					int lightidx = (int)std::floor(-9 * (light | _n) / _n.Norm());

					lightidx = std::max(0, lightidx);

					Pixel p1 = { (int)_pt1.x, (int)_pt1.y };
					Pixel p2 = { (int)_pt2.x, (int)_pt2.y };
					Pixel p3 = { (int)_pt3.x, (int)_pt3.y };

					//FillTriangle(_pt1, _pt2, _pt3, grayscale[lightidx]);
					FillTriangle({ p1, p2, p3 }, grayscale[lightidx]);

					//ROI roi = { {0, 0}, { (int)m_Width, (int)m_Height } };
					//FillTriangleRecursive({ p1, p2, p3 }, roi, grayscale[lightidx]);

					/*
					DrawLine(_pt1, _pt2, '.');
					DrawLine(_pt2, _pt3, '.');
					DrawLine(_pt3, _pt1, '.');
					*/
				}
			}
#endif // DRAW_FACES

#ifdef DRAW_EDGES
			// Loop through edges
			for(const Model3D::Edge& edge : model.Edges())
			{
				const HVector3Df& nface1  = model.Normals()[edge.n1];
				const HVector3Df& nface2  = model.Normals()[edge.n2];

				HVector3Df cam2v1 = model.Vertices()[edge.v1] - planesfromObj[1].second; // CamPos to point

				if ((cam2v1 | nface1) > 0.0f && (cam2v1 | nface2) > 0.0f)
					continue;

				HVector3Df v1 = model.Vertices()[edge.v1];
				HVector3Df v2 = model.Vertices()[edge.v2];
				static HVector3Df o1(0.0f, 0.0f, 0.0f);
				static HVector3Df o2(0.0f, 0.0f, 0.0f);

				bool outoffield = false;

#ifdef DRAW_FACES
				char symbol = ' ';
#else
				char symbol = 0;
#endif

				for (auto& p : planesfromObj)
				{
					uint num = ClipEdge(v1, v2, p.first, p.second, o1, o2);

					if (outoffield = (num <= 0))
						break;

					//if (num == 1)
					//	symbol = '.';

					v1 = o1;
					v2 = o2;
				}

				if (outoffield)
					continue;

				HVector2Df _pt1 = _Proj * (CamToObj * o1).mat;
				HVector2Df _pt2 = _Proj * (CamToObj * o2).mat;

				_pt1.Homogenize();
				_pt2.Homogenize();

				DrawLine(_pt1, _pt2, symbol);
			}
#endif // DRAW_EDGES
		}

		HeartBeat();

		ENDCHRONO;

		sstr.str(std::string());
		sstr << "Position : (" << m_R0ToCam.Tx << ", " << m_R0ToCam.Ty << ", " << m_R0ToCam.Tz << ")";

		DisplayMessage(sstr.str(), Slots::_1);

		sstr.str(std::string());
		sstr << "Ellapsed time : " << (float)ellapsed_micros / 1000.0f << " ms";

		DisplayMessage(sstr.str(), Slots::_3);

		sstr.str(std::string());
		sstr << "FPS : " << 1.0f / ellapsedtime.count();

		DisplayMessage(sstr.str(), Slots::_5);

		Render();

		/*
		a += aspeed * dt;

		R0ToObjs[1].Rux =  std::cos(TORAD(a));
		R0ToObjs[1].Ruy =  std::sin(TORAD(a));
		R0ToObjs[1].Rvx = -std::sin(TORAD(a));
		R0ToObjs[1].Rvy =  std::cos(TORAD(a));
		*/
	}
}
#endif // !TEST_CONSOLE

Console& Console::Get() noexcept
{
	static Console console;
	return console;
}

void Console::Start()
{
	// Start up Console device
	ConstructConsole(150, 100, 8, 8);

	// Lauch thread
	SyncDevice::Start();
}

void Console::Clear()
{
	std::memset(m_ScreenBuffer, 0, sizeof(CHAR_INFO) * m_Width * m_Height);
}

char Console::GetPixelValue(int x, int y) const
{
	// TODO : Check if x and y are not out of range !!!
	ASSERT(x >= 0);
	ASSERT(x < m_Width);
	ASSERT(y >= 0);
	ASSERT(y < m_Height);

	return m_ScreenBuffer[x + y * m_Width].Char.AsciiChar;
}

void Console::DrawPoint(int x, int y, short c, short col)
{
	if (x < 0 || x >= m_Width)
		return;

	if (y < 0 || y >= m_Height)
		return;

	m_ScreenBuffer[x + y * m_Width].Char.UnicodeChar = c;
	m_ScreenBuffer[x + y * m_Width].Attributes       = col;
}

HVector3Df Console::SegmentPlaneIntersection(const HVector3Df& v1, const HVector3Df& v2, const HVector3Df& n, const HVector3Df& p)
{
	HVector3Df pv1  = v1 - p;
	HVector3Df v1v2 = v2 - v1;

	float k = -(pv1 | n) / (v1v2 | n);

	return k * v1v2 + v1;
}

uint Console::ClipEdge(const HVector3Df& v1, const HVector3Df& v2, // Edge
	                   const HVector3Df& n,  const HVector3Df& p,  // Plane parameters
	                         HVector3Df& o1,       HVector3Df& o2)
{
	std::array<const HVector3Df*, 2> vertices = { &v1, &v2 };

	auto it = std::partition(vertices.begin(), vertices.end(), [&n, &p](const HVector3Df* v)
	{
		return ((*v - p) | n) > 0.0f;
	});

	uint num = it - vertices.begin();

	if (num == 1)
	{
		o1 = *vertices[0];
		o2 = SegmentPlaneIntersection(*vertices[0], *vertices[1], n, p);
	}
	else
		o1 = *vertices[0], o2 = *vertices[1];

	return num;
}

uint Console::ClipTriangle(const Triangle& in_t, const HVector3Df& n, const HVector3Df& p, Triangle& o_t1, Triangle& o_t2)
{
	std::array<const HVector3Df*, 3> vertices = { &in_t.vertices[0], &in_t.vertices[1], &in_t.vertices[2] };

	auto it = std::partition(vertices.begin(), vertices.end(), [&n, &p](const HVector3Df* v) {
		return ((*v - p) | n) > 0.0f;
	});

	uint num = it - vertices.begin();

	if (num == 0 || num == 3)
		return num;

	switch (num)
	{
	case 1: // One point visible
		o_t1.vertices[0] = *vertices[0];
		o_t1.vertices[1] = SegmentPlaneIntersection(*vertices[0], *vertices[1], n, p);
		o_t1.vertices[2] = SegmentPlaneIntersection(*vertices[0], *vertices[2], n, p);

		break;
	case 2: // Two points visible
		o_t1.vertices[0] = *vertices[0];
		o_t1.vertices[1] = *vertices[1];
		o_t1.vertices[2] = SegmentPlaneIntersection(*vertices[1], *vertices[2], n, p);

		o_t2.vertices[0] = *vertices[0];
		o_t2.vertices[1] = SegmentPlaneIntersection(*vertices[1], *vertices[2], n, p);
		o_t2.vertices[2] = SegmentPlaneIntersection(*vertices[0], *vertices[2], n, p);

		break;
	default:
		break;
	}

	return num;
}

void Console::DrawLine(int x1, int y1, int x2, int y2, short c, short color)
{
	// Adapted from OneLoneCoder
	// Original code can be found at https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h

	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

	dx  = x2 - x1;
	dy  = y2 - y1;
	dx1 = abs(dx);
	dy1 = abs(dy);
	px  = 2 * dy1 - dx1;
	py  = 2 * dx1 - dy1;

	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}

		DrawPoint(x, y, c, color);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;

			if (px<0)
				px = px + 2 * dy1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0))
					y = y + 1;
				else
					y = y - 1;

				px = px + 2 * (dy1 - dx1);
			}

			DrawPoint(x, y, c, color);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}

		DrawPoint(x, y, c, color);

		for (i = 0; y<ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0))
					x = x + 1;
				else
					x = x - 1;

				py = py + 2 * (dx1 - dy1);
			}

			DrawPoint(x, y, c, color);
		}
	}
}

void Console::DrawLine(const HVector2Df& v1, const HVector2Df& v2, char c)
{
	DrawLine(v1.x, v1.y, v2.x, v2.y, c);
}

void Console::FillRectangle(const Pixel& TL, const Pixel& BR, char c, const std::function<bool(const Pixel&)>& criteria)
{
	ASSERT(TL.x <= BR.x);
	ASSERT(TL.y <= BR.y);

	for (int i = TL.x; i < BR.x; ++i)
		for (int j = TL.y; j < BR.y; ++j)
			if (criteria({ i, j }))
				DrawPoint(i, j, c);
}

void Console::FillTriangle(const HVector2Df& v1, const HVector2Df& v2, const HVector2Df& v3, char c)
{
	auto insidetriangle = [&](const HVector2Df& p)
	{
		HVector2Df v1v2 = v2 - v1; HVector2Df n1 = { -v1v2.y, v1v2.x };
		HVector2Df v2v3 = v3 - v2; HVector2Df n2 = { -v2v3.y, v2v3.x };
		HVector2Df v3v1 = v1 - v3; HVector2Df n3 = { -v3v1.y, v3v1.x };

		short s1 = sign((p - v1) | n1);
		short s2 = sign((p - v2) | n2);
		short s3 = sign((p - v3) | n3);

		return s1 == s2 && s2 == s3;
	};

	auto insidescreen = [&](const HVector2Df& p)
	{
		bool a = (uint32_t)p.x >= 0 && (uint32_t)p.x < this->Width();
		bool b = (uint32_t)p.y >= 0 && (uint32_t)p.y < this->Height();

		return a && b;
	};

	DrawPoint((uint32_t)v1.x, (uint32_t)v1.y, c);
	DrawPoint((uint32_t)v2.x, (uint32_t)v2.y, c);
	DrawPoint((uint32_t)v3.x, (uint32_t)v3.y, c);

	std::queue<HVector2Df> queue;

	queue.push((v1 + v2 + v3) / 3.0f);

	DrawPoint((uint32_t)queue.front().x, (uint32_t)queue.front().y, c);

	while (!queue.empty())
	{
		uint32_t x = queue.front().x;
		uint32_t y = queue.front().y;

		queue.pop();

		HVector2Df voisins[] = {
			HVector2Df(x + 1, y),
			HVector2Df(x,     y + 1),
			HVector2Df(x - 1, y),
			HVector2Df(x,     y - 1)
		};

		for (const HVector2Df& pt : voisins)
		{
			if (!insidescreen(pt))
				continue;

			if (GetPixelValue((uint32_t)pt.x, (uint32_t)pt.y) == 0)
			{
				if (insidetriangle(pt))
				{
					DrawPoint((uint32_t)pt.x, (uint32_t)pt.y, c);
					queue.push(pt);
				}
			}
		}
	}
}

void Console::FillTriangle(const Triangle2D& triangle, char c)
{
	Pixel TL = {
		std::min({ triangle.p1.x, triangle.p2.x,triangle.p3.x }),
		std::min({ triangle.p1.y, triangle.p2.y,triangle.p3.y })
	};

	Pixel BR = {
		std::max({ triangle.p1.x, triangle.p2.x, triangle.p3.x }) + 1,
		std::max({ triangle.p1.y, triangle.p2.y, triangle.p3.y }) + 1
	};

	static const Pixel offset = { 1, 1 };

	Triangle2D triangleX2 = {
		2 * triangle.p1 + offset,
		2 * triangle.p2 + offset,
		2 * triangle.p3 + offset
	};

	FillRectangle(TL, BR, c, [&](const Pixel& p)
		{
			Pixel pX2 = 2 * p + offset;

			Pixel n1 = (triangleX2.p2 - triangleX2.p1).NormalVector();
			Pixel n2 = (triangleX2.p3 - triangleX2.p2).NormalVector();
			Pixel n3 = (triangleX2.p1 - triangleX2.p3).NormalVector();

			short s1 = sign((pX2 - triangleX2.p1) | n1);
			short s2 = sign((pX2 - triangleX2.p2) | n2);
			short s3 = sign((pX2 - triangleX2.p3) | n3);

			return (s1 == s2) && (s2 == s3);
		});
}

int Console::ROI::Width() const
{
	return BR.x - TL.x;
}

int Console::ROI::Height() const
{
	return BR.y - TL.y;
}

Console::Pixel Console::ROI::TR() const
{
	return { BR.x, TL.y };
}

Console::Pixel Console::ROI::BL() const
{
	return { TL.x, BR.y };
}

void Console::FillTriangleRecursive(const Triangle2D& triangle, const ROI& aabb, char c)
{
	// Temporary
	//FillRectangle(aabb.TL, aabb.BR, c);
	//RENDER_AND_WAIT;
	//++c;

	auto LineAABBInter = [](const Pixel& p1, const Pixel& p2, const ROI& aabb)
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

	auto InsideAABB = [](const Pixel& p, const ROI& aabb)
	{
		const Pixel offset = { 1, 1 };

		Pixel pX2 = 2 * p + offset;

		Pixel TL = 2 * aabb.TL;
		Pixel BR = 2 * (aabb.BR + offset);

		bool a = (pX2.x >= TL.x) && (pX2.x < BR.x);
		bool b = (pX2.y >= TL.y) && (pX2.y < BR.y);

		return a && b;
	};

	int A, B, C;

	A = LineAABBInter(triangle.p1, triangle.p2, aabb);
	B = LineAABBInter(triangle.p2, triangle.p3, aabb);
	C = LineAABBInter(triangle.p3, triangle.p1, aabb);

	if (A < 0 || B < 0 || C < 0)
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

	//std::stringstream sstr;
	//
	//sstr << "Case : " << (int)count;
	//
	//DisplayMessage(sstr.str(), Slots::_5);

	switch (count)
	{
	case 0:
	case 2:
	{
		//BREACKIF(aabb.BR.x - aabb.TL.x == 2 || aabb.BR.y - aabb.TL.y == 2);

		if ((aabb.Width() <= 1) && (aabb.Height() <= 1))
		{
			if (InsideTriangle(triangle, aabb.TL))
				DrawPoint(aabb.TL.x, aabb.TL.y, c);

			return;
		}

		int midx = (aabb.TL.x + aabb.BR.x) / 2;
		int midy = (aabb.TL.y + aabb.BR.y) / 2;

		ROI subaabbs[] = {
			{ { aabb.TL.x, aabb.TL.y }, { midx,      midy      } },
			{ { midx,      aabb.TL.y }, { aabb.BR.x, midy      } },
			{ { aabb.TL.x, midy      }, { midx,      aabb.BR.y } },
			{ { midx,      midy      }, { aabb.BR.x, aabb.BR.y } }
		};

		for (const ROI& _aabb : subaabbs)
			FillTriangleRecursive(triangle, _aabb, c);

		break;
	}
	case 1:
	{
		//RENDER_AND_WAIT;

		if ((aabb.Width() <= 1) && (aabb.Height() <= 1))
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

		if (!inter)
			break;

		int midx = (aabb.TL.x + aabb.BR.x) / 2;
		int midy = (aabb.TL.y + aabb.BR.y) / 2;

		ROI subaabbs[] = {
			{ { aabb.TL.x, aabb.TL.y }, { midx,      midy      } },
			{ { midx,      aabb.TL.y }, { aabb.BR.x, midy      } },
			{ { aabb.TL.x, midy      }, { midx,      aabb.BR.y } },
			{ { midx,      midy      }, { aabb.BR.x, aabb.BR.y } }
		};

		for (const ROI& _aabb : subaabbs)
			FillTriangleRecursive(triangle, _aabb, c);

		break;
	}
	case 3:
	{
		FillRectangle(aabb.TL, aabb.BR, c);

		break;
	}
	default:
		break;
	}

	//RENDER_AND_WAIT;
	//Render();
}

void Console::DisplayMessage(const std::string& msg, Slots slot)
{
	for (UINT32 i = 0; i < msg.length(); i++)
		DrawPoint(i + 1, m_Height - 1 - slot, msg[i]);
}

void Console::HeartBeat()
{
	static UINT32 count = 0;

	++count;

	if (count > 120)
		count = 0;

	DrawPoint(m_Width - 2, m_Height - 2, (count <= 60 ? '0' : ' '));
}

void Console::Render()
{
	//m_ScreenBuffer[m_Width * m_Height - 1].Char.UnicodeChar = 0;
	//WriteConsoleOutputCharacter(m_HConsole, m_ScreenBuffer, m_Width * m_Height, { 0,0 }, &m_DwBytesWritten);
	WriteConsoleOutput(m_HConsole, m_ScreenBuffer, { (short)m_Width, (short)m_Height }, { 0, 0 }, &m_RectWindow);
}
