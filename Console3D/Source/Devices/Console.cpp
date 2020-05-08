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
	//m_Focal(150.0f)
	m_Focal(90.0f)
{
	m_Screen = new char[m_Width * m_Height];
	m_HConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(m_HConsole);

	m_MainThread = std::thread(&Console::MainThread, this);
}

Console::~Console()
{
	delete[] m_Screen;

	m_MainThread.join();
}

void Console::MainThread()
{
	PaceMaker& pacemaker = PaceMaker::Get();
	
	float aspeed = 36.0f;			// 1 tour / 10s
	float dt     = 16.0f / 1000.0f;	// Delta de temps
	float a      = 0.0f;			// Angle

	float radius = 35;

	Model3D models[] = {
		OBJReader().ReadFile<Model3D>("Ressource/carpet.obj", false),
		/*
		OBJReader().ReadFile<Model3D>("Ressource/axis.obj", true)
		OBJReader().ReadFile<Model3D>("Ressource/cube.obj", false)
		OBJReader().ReadFile<Model3D>("Ressource/teapot.obj", true)
		*/
		OBJReader().ReadFile<Model3D>("Ressource/octogon.obj", false)
	};

 	const float scalefactor = 1.0f;

	// Scaling up octogon
	for (HVector3D& v : models[1].Vertices())
	{
		v.x *= scalefactor;
		v.y *= scalefactor;
		v.z *= scalefactor;
	}

	Transform3D R0ToObjs[] = {
		Transform3D(),
		Transform3D()
	};

	// Set up Camera transform

	m_R0ToCam.Rux =  1.0f;
	m_R0ToCam.Ruy =  0.0f;
	m_R0ToCam.Ruz =  0.0f;
	m_R0ToCam.Rvx =  0.0f;
	m_R0ToCam.Rvy =  0.0f;
	m_R0ToCam.Rvz =  1.0f;
	m_R0ToCam.Rwx =  0.0f;
	m_R0ToCam.Rwy = -1.0f;
	m_R0ToCam.Rwz =  0.0f;
	m_R0ToCam.Tx  =  0.0f;
	m_R0ToCam.Ty  =  10.0f;
	m_R0ToCam.Tz  =  1.8f;

	const float tab[3][4] = {
		m_Focal, 0.0f,    0.0f,  0.0f,
		0.0f,    m_Focal, 0.0f,  0.0f,
		0.0f,    0.0f,    1.0f,  0.0f,
	};

	LCNMath::Matrix::StaticMatrix::Matrix<float, 3, 4> Projection(tab);

	Transform2D ImgToCam(90.0f, 60.0f, 180.0f);

	// For clipping

	// TODO : clean up
	/*
	HVector3D nh1(0,        m_Focal, m_Height / 2, false);
	HVector3D nh2(0,       -m_Focal, m_Height / 2, false);
	HVector3D nw1(-m_Focal, 0,       m_Width / 2,  false);
	HVector3D nw2( m_Focal, 0,       m_Width / 2,  false);
	*/

	std::pair<HVector3D, HVector3D> planes[] = {
		{ HVector3D(HVector3D::Z()),                         HVector3D(0.0f, 0.0f, 0.5f) },
		{ HVector3D( 0,       m_Focal, m_Height / 2, false), HVector3D(0.0f, 0.0f, 0.0f) },
		{ HVector3D( 0,      -m_Focal, m_Height / 2, false), HVector3D(0.0f, 0.0f, 0.0f) },
		{ HVector3D(-m_Focal, 0,       m_Width  / 2, false), HVector3D(0.0f, 0.0f, 0.0f) },
		{ HVector3D( m_Focal, 0,       m_Width  / 2, false), HVector3D(0.0f, 0.0f, 0.0f) }
	};

	// Console device loop
	while (pacemaker.Heartbeat())
	{		
		Clear();

		Transform3D CamToR0 = m_R0ToCam.mat.Invert();

		STARTCHRONO;

		for (uint i = 0; i < 2; i++)
		{
			const Model3D& model = models[i];

			Transform3D CamToObj = CamToR0 * R0ToObjs[i];

			//LCNMath::Matrix::StaticMatrix::Matrix<float, 3, 4> _Proj = ImgToCam.mat * Projection * CamToObj.mat;
			LCNMath::Matrix::StaticMatrix::Matrix<float, 3, 4> _Proj = ImgToCam.mat * Projection;

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
			for(const Model3D::Edge& edge : model.Edges())
			{
				HVector3D v1 = CamToObj * model.Vertices()[edge.v1];
				HVector3D v2 = CamToObj * model.Vertices()[edge.v2];
				HVector3D o1(0.0f, 0.0f, 0.0f);
				HVector3D o2(0.0f, 0.0f, 0.0f);

				bool outoffield = false;
				char symbol = '#';

				for (auto& p : planes)
				{
					uint num = ClipEdge(v1, v2, p.first, p.second, o1, o2);

					if (outoffield = (num <= 0))
						break;

					if (num == 1)
						symbol = '.';

					v1 = o1;
					v2 = o2;
				}

				if (outoffield)
					continue;

				HVector3D nface1  = CamToObj * model.Normals()[edge.n1];
				HVector3D nface2  = CamToObj * model.Normals()[edge.n2];

				if ((o1 | nface1) > 0.0f && (o1 | nface2) > 0.0f)
					continue;

				HVector2D _pt1 = _Proj * o1.mat;
				HVector2D _pt2 = _Proj * o2.mat;

				_pt1.Homogenize();
				_pt2.Homogenize();

				DrawLine(_pt1, _pt2, symbol);
			}
#endif // DRAW_EDGES
		}

		ENDCHRONO;

		HeartBeat();

		std::stringstream sstr;

		sstr << "Ellapsed time : " << (float)ellapsed_micros / 1000.0f << " ms";

		DisplayMessage(sstr.str());

		Render();

		a += aspeed * dt;

		R0ToObjs[1].Rux =  std::cos(TORAD(a));
		R0ToObjs[1].Ruy =  std::sin(TORAD(a));
		R0ToObjs[1].Rvx = -std::sin(TORAD(a));
		R0ToObjs[1].Rvy =  std::cos(TORAD(a));
	}
}

Console& Console::Get()
{
	static Console console;
	return console;
}

void Console::Clear()
{
	for (unsigned int i = 0; i < m_Width * m_Height; i++)
		m_Screen[i] = 0;
}

bool Console::ScreenPlaneProjection(const HVector3D& a, const HVector3D& b, HVector3D& pa, HVector3D& pb) const
{
	if (sign(a.z - cm_ScreenDist) < 0.0f && sign(b.z - cm_ScreenDist) < 0.0f)
		return false;

	pa = (a.z > 0.0f ? a : b);
	pb = (a.z > 0.0f ? b : a);

	float dz = pb.z - pa.z;

	if (std::abs(dz) < 0.001f)
		return true;

	float k = (cm_ScreenDist - pb.z) / dz;

	pb = pb + k * (pb - pa);

	return true;
}

void Console::DrawPoint(float x, float y, char c)
{
	if (x < 0 || x >= m_Width)
		return;

	if (y < 0 || y >= m_Height)
		return;

	m_Screen[(INT32)x + (INT32)y * m_Width] = c;
}

bool Console::LineInSight(HVector2D& OA, HVector2D& OB)
{
	static const HVector2D TL = { 0.0f,           0.0f };
	static const HVector2D TR = { (float)m_Width, 0.0f };
	static const HVector2D BL = { 0.0f,           (float)m_Height };
	static const HVector2D BR = { (float)m_Width, (float)m_Height };

	HVector2D AB = OB - OA;
	HVector2D n = { -AB.y, AB.x };

	float _1 = n | (OA - TL);
	float _2 = n | (OA - TR);
	float _3 = n | (OA - BL);
	float _4 = n | (OA - BR);

	if (sign(_1) == sign(_2) &&
		sign(_1) == sign(_3) &&
		sign(_1) == sign(_4))
		return false;

	HVector2D _OA = OA;
	HVector2D _OB = OB;

	float W = (float)m_Width;
	float H = (float)m_Height;

	float t0y = -1000000;
	float tWy =  1000000;
	float tx0 = -1000000;
	float txH =  1000000;

	if (std::abs(AB.x) > 0.01f)
	{
		t0y = -_OA.x / (AB.x);
		tWy = (W - _OA.x) / (AB.x);
	}

	if (std::abs(AB.y) > 0.01f)
	{
		tx0 = -_OA.y / (AB.y);
		txH = (H - _OA.y) / (AB.y);
	}

	std::vector<float> tab = { 0.0f, 1.0f, t0y, tWy, tx0, txH };
	std::sort(tab.begin(), tab.end());

	if (tab[2] >= 1.0f || tab[3] <= 0.0f)
		return false;

	OA = _OA + tab[2] * AB;
	OB = _OA + tab[3] * AB;

	return true;
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

		float k = -(pv1 | n) / (v1v2 | n);

		o2 = k * v1v2 + o1;
	}
	else
		o1 = *vertices[0], o2 = *vertices[1];

	return num;
}

void Console::DrawLine(const HVector2D& v1, const HVector2D& v2, char c)
{
	float x1 = v1.PX();
	float y1 = v1.PY();
	float x2 = v2.PX();
	float y2 = v2.PY();

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