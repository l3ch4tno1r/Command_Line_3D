#include "Console.h"

#include "Utilities\ErrorHandling.h"
#include "Utilities\Angles.h"
#include "Utilities\TimeMeasurement.h"
#include "Utilities\Utils.h"

#include "Devices\PaceMaker.h"

#include "Geometry\Geometry2D\Transform2D.h"
//#include "Geometry\Geometry2D\HVector2D.h"
//#include "Geometry\Geometry3D\Transform3D.h"
//#include "Geometry\Geometry3D\HVector3D.h"

#include "Graphics\OBJReader.h"

#include <chrono>
#include <cmath>
#include <sstream>
#include <array>
#include <algorithm>
#include <utility>

/*
#define DRAW_FACES
*/
#define DRAW_EDGES

Console::Console() :
	m_Width(180),
	m_Height(120),
	m_Screen(nullptr),
	m_HConsole(nullptr),
	m_DwBytesWritten(0),
	m_Focal(120.0f)
	//m_Focal(90.0f)
{
	m_Screen = new char[m_Width * m_Height];
	m_HConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(m_HConsole);

	// Set up Camera transform
	m_R0ToCam.Rux = 1.0f;
	m_R0ToCam.Ruy = 0.0f;
	m_R0ToCam.Ruz = 0.0f;
	m_R0ToCam.Rvx = 0.0f;
	m_R0ToCam.Rvy = 0.0f;
	m_R0ToCam.Rvz = 1.0f;
	m_R0ToCam.Rwx = 0.0f;
	m_R0ToCam.Rwy = -1.0f;
	m_R0ToCam.Rwz = 0.0f;
	m_R0ToCam.Tx = 0.0f;
	m_R0ToCam.Ty = 10.0f;
	m_R0ToCam.Tz = 1.8f;

	// Lauch thread
	m_MainThread = std::thread(&Console::MainThread, this);
}

Console::~Console()
{
	m_MainThread.join();

	delete[] m_Screen;
}

void Console::MainThread()
{
	PaceMaker& pacemaker = PaceMaker::Get();
	
	/*
	float aspeed = 36.0f;			// 1 tour / 10s
	float dt     = 16.0f / 1000.0f;	// Delta de temps
	float a      = 0.0f;			// Angle
	*/

	float radius = 35;

	Model3D models[] = {
		OBJReader().ReadFile<Model3D>("Ressource/null.obj", false),
		/*
		OBJReader().ReadFile<Model3D>("Ressource/carpet.obj", false),
		OBJReader().ReadFile<Model3D>("Ressource/cube.obj", false)
		OBJReader().ReadFile<Model3D>("Ressource/debug.obj", true)
		OBJReader().ReadFile<Model3D>("Ressource/octogon_no_normals.obj", true)
		OBJReader().ReadFile<Model3D>("Ressource/teapot.obj", true)
		OBJReader().ReadFile<Model3D>("Ressource/table_basique.obj", false)
		*/
		OBJReader().ReadFile<Model3D>("Ressource/axisr.obj", true)
	};

	// Quick fix for teapot
	/*
	Transform3D teapot({
		1.0f, 0.0f,  0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 1.0f,  0.0f, 0.0f,
		0.0f, 0.0f,  0.0f, 1.0f
	});

	for (HVector3D& vertex : models[1].Vertices())
		vertex = teapot * vertex;

	for (HVector3D& vertex : models[1].Normals())
		vertex = teapot * vertex;
	*/

	//const float scalefactor = 1.0f;
	const float scalefactor[] = { 1.0f, 1.0f };

	// Scaling models
	for (uint32_t i = 0; i < 2; ++i)
	{
		for (HVector3D& v : models[i].Vertices())
		{
			v.x *= scalefactor[i];
			v.y *= scalefactor[i];
			v.z *= scalefactor[i];
		}
	}

	Transform3D R0ToObjs[] = {
		Transform3D(),
		Transform3D()
	};

	const float tab[3][4] = {
		m_Focal, 0.0f,    0.0f,  0.0f,
		0.0f,    m_Focal, 0.0f,  0.0f,
		0.0f,    0.0f,    1.0f,  0.0f,
	};

	LCNMath::Matrix::StaticMatrix::Matrix<float, 3, 4> Projection(tab);

	//Transform2D ImgToCam(90.0f, 60.0f, 180.0f);
	Transform2D ImgToCam(m_Width / 2, m_Height / 2, 180.0f);

	// For clipping

	std::pair<HVector3D, HVector3D> planes[] = {
		{ HVector3D(HVector3D::Z()),                         HVector3D(0.0f, 0.0f, 0.5f) },
		{ HVector3D( 0,       m_Focal, m_Height / 2, false), HVector3D(0.0f, 0.0f, 0.0f) },
		{ HVector3D( 0,      -m_Focal, m_Height / 2, false), HVector3D(0.0f, 0.0f, 0.0f) },
		{ HVector3D(-m_Focal, 0,       m_Width  / 2, false), HVector3D(0.0f, 0.0f, 0.0f) },
		{ HVector3D( m_Focal, 0,       m_Width  / 2, false), HVector3D(0.0f, 0.0f, 0.0f) }
	};

	std::pair<HVector3D, HVector3D> planesfromObj[] = {
		{ HVector3D(0.0f, 0.0f, 0.0f, false), HVector3D(0.0f, 0.0f, 0.0f)},
		{ HVector3D(0.0f, 0.0f, 0.0f, false), HVector3D(0.0f, 0.0f, 0.0f)},
		{ HVector3D(0.0f, 0.0f, 0.0f, false), HVector3D(0.0f, 0.0f, 0.0f)},
		{ HVector3D(0.0f, 0.0f, 0.0f, false), HVector3D(0.0f, 0.0f, 0.0f)},
		{ HVector3D(0.0f, 0.0f, 0.0f, false), HVector3D(0.0f, 0.0f, 0.0f)}
	};

	LCNMath::Matrix::StaticMatrix::Matrix<float, 3, 4> _Proj = ImgToCam.mat * Projection;

	// FPS measurement
	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	// Console device loop
	while (pacemaker.Heartbeat(1))
	{
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> ellapsedtime = tp2 - tp1;
		tp1 = tp2;

		Clear();

		Transform3D CamToR0 = m_R0ToCam.mat.Invert();

		for (uint i = 0; i < 2; i++)
		{
			const Model3D& model = models[i];

			Transform3D CamToObj = CamToR0 * R0ToObjs[i];
			Transform3D ObjToCam = CamToObj.mat.Invert();

			for (uint i = 0; i < 5; i++)
			{
				planesfromObj[i].first  = ObjToCam * planes[i].first;
				planesfromObj[i].second = ObjToCam * planes[i].second;
			}

#ifdef DRAW_FACES
			for (const Model3D::Face& face : model.Faces())
			{
				HVector3D vertices[3] = {
					CamToObj * model.Vertices()[face.v1],
					CamToObj * model.Vertices()[face.v2],
					CamToObj * model.Vertices()[face.v3]
				};

				HVector3D nface = CamToObj * model.Normals()[face.vn1];

				if ((vertices[0] | nface) > 0.0f)
					continue;

				for (uint i = 0; i < 3; i++)
				{
					uint ip1 = (i + 1) % 3;

					HVector3D p1(true), p2(true);

					if (!ScreenPlaneProjection(vertices[i], vertices[ip1], p1, p2))
						continue;

					HVector2D _pt1 = _Proj * model.Vertices()[face.Vertices[i]].mat;
					HVector2D _pt2 = _Proj * model.Vertices()[face.Vertices[ip1]].mat;

					_pt1.Homogenize();
					_pt2.Homogenize();

					DrawLine(_pt1, _pt2);
				}
			}
#endif // DRAW_FACES

#ifdef DRAW_EDGES
			// Loop through edges
			for(const Model3D::Edge& edge : model.Edges())
			{
				TIMER(t1);

				HVector3D v1 = model.Vertices()[edge.v1];
				HVector3D v2 = model.Vertices()[edge.v2];
				static HVector3D o1(0.0f, 0.0f, 0.0f);
				static HVector3D o2(0.0f, 0.0f, 0.0f);

				REGISTERTIME(t1);

				bool outoffield = false;
				char symbol = '#';

				for (auto& p : planesfromObj)
				{
					uint num = ClipEdge(v1, v2, p.first, p.second, o1, o2);

					if (outoffield = (num <= 0))
						break;

					if (num == 1)
						symbol = '.';

					v1 = o1;
					v2 = o2;
				}

				REGISTERTIME(t1);

				if (outoffield)
					continue;

				HVector3D nface1  = model.Normals()[edge.n1];
				HVector3D nface2  = model.Normals()[edge.n2];
				HVector3D _o1     = o1 - planesfromObj[1].second; // CamPos to point

				if ((_o1 | nface1) > 0.0f && (_o1 | nface2) > 0.0f)
					continue;

				HVector2D _pt1 = _Proj * (CamToObj * o1).mat;
				HVector2D _pt2 = _Proj * (CamToObj * o2).mat;

				_pt1.Homogenize();
				_pt2.Homogenize();

				REGISTERTIME(t1);

				DrawLine(_pt1, _pt2, symbol);

				REGISTERTIME(t1);
			}
#endif // DRAW_EDGES
		}

		HeartBeat();

		std::stringstream sstr;

		//sstr << "Ellapsed time : " << (float)ellapsed_micros / 1000.0f << " ms";
		sstr << "Position : (" << m_R0ToCam.Tx << ", " << m_R0ToCam.Ty << ", " << m_R0ToCam.Tz << ") - FPS : " << 1.0f / ellapsedtime.count();

		DisplayMessage(sstr.str());

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

Console& Console::Get()
{
	static Console console;
	return console;
}

void Console::Clear()
{
	memset(m_Screen, 0, sizeof(char) * m_Width * m_Height);
}

void Console::DrawPoint(int x, int y, char c)
{
	if (x < 0 || x >= m_Width)
		return;

	if (y < 0 || y >= m_Height)
		return;

	m_Screen[x + y * m_Width] = c;
}

uint Console::ClipEdge(const HVector3D& v1, const HVector3D& v2, // Edge
	                   const HVector3D& n,  const HVector3D& p,  // Plane parameters
	                         HVector3D& o1,       HVector3D& o2)
{
	std::array<const HVector3D*, 2> vertices = { &v1, &v2 };

	auto it = std::partition(vertices.begin(), vertices.end(), [&n, &p](const HVector3D* v)
	{
		return ((*v - p) | n) > 0.0f;
	});

	uint num = it - vertices.begin();

	if (num == 1)
	{
		o1 = *vertices[0];

		HVector3D pv1 = o1 - p;
		HVector3D v1v2 = *vertices[1] - o1;

		// TODO : Manage the case where v1v2 belongs to the plane ?
		float k = -(pv1 | n) / (v1v2 | n);

		o2 = k * v1v2 + o1;
	}
	else
		o1 = *vertices[0], o2 = *vertices[1];

	return num;
}

uint Console::ClipTriangle(const Triangle& in_t, const HVector3D& n, const HVector3D& p, Triangle& o_t1, Triangle& o_t2)
{
	std::array<const HVector3D*, 3> vertices = { &in_t.vertices[0], &in_t.vertices[1], &in_t.vertices[2] };

	auto it = std::partition(vertices.begin(), vertices.end(), [&n, &p](const HVector3D* v) {
		return ((*v - p) | n) > 0.0f;
	});

	uint num = it - vertices.begin();

	return num;
}

void Console::DrawLine(int x1, int y1, int x2, int y2, char c)
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

		DrawPoint(x, y, c);

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

			DrawPoint(x, y, c);
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

		DrawPoint(x, y, c);

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

			DrawPoint(x, y, c);
		}
	}
}

void Console::DrawLine(const HVector2D& v1, const HVector2D& v2, char c)
{
	/*
	float dx = x2 - x1;
	float dy = y2 - y1;

	if (dx == 0 && dy == 0)
	{
		DrawPoint(x1, y1, c);
		return;
	}

	float stepx, stepy;

	if (abs(dx) > abs(dy))
	{
		stepx = (dx > 0 ? 1.0f : -1.0f);
		stepy = dy / abs(dx);
	}
	else
	{
		stepx = dx / abs(dy);
		stepy = (dy > 0 ? 1.0f : -1.0f);
	}

	float X = x1;
	float Y = y1;

	while ((abs(X - x1) + abs(Y - y1)) < (abs(x2 - x1) + abs(y2 - y1)))
	{
		X += stepx;
		Y += stepy;

		DrawPoint(X, Y, c);
	}

	DrawPoint(x1, y1, c);
	DrawPoint(x2, y2, c);
	*/

	DrawLine(v1.x, v1.y, v2.x, v2.y, c);
}

void Console::DisplayMessage(const std::string & msg)
{
	for (UINT32 i = 0; i < msg.length(); i++)
		DrawPoint(i + 1, m_Height - 2, msg[i]);
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
	m_Screen[m_Width * m_Height - 1] = '\0';
	WriteConsoleOutputCharacter(m_HConsole, m_Screen, m_Width * m_Height, { 0,0 }, &m_DwBytesWritten);
}

// Instanciation
//Console Console::console;
