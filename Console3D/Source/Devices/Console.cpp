#include "Console.h"

#include "Utilities\ErrorHandling.h"
#include "Utilities\Angles.h"
#include "Utilities\TimeMeasurement.h"

#include "Devices\PaceMaker.h"

#include "Geometry\Geometry2D\Transform2D.h"
#include "Geometry\Geometry2D\Vector2D.h"
#include "Geometry\Geometry3D\Transform3D.h"
#include "Geometry\Geometry3D\HVector3D.h"

#include "Graphics\OBJReader.h"

#include <chrono>
#include <cmath>
#include <sstream>

#define CONSOLETEST02

Console::Console() :
	m_Width(180),
	m_Height(120),
	m_Screen(nullptr),
	m_HConsole(nullptr),
	m_DwBytesWritten(0),
	m_Focal(180.0f)
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
		OBJReader().ReadFile("Ressource/carpet.obj"),
		OBJReader().ReadFile("Ressource/octogon.obj")
	};

	const float scalefactor = 1.0f;

	// Scaling up octogon
	for (HVector3D& v : models[1].Vertices())
	{
		v.x *= scalefactor;
		v.y *= scalefactor;
		v.z *= scalefactor;
	}

	Transform3D ObjsFromR0[] = {
		Transform3D(),
		Transform3D()
	};

	// Set up Camera transform

	m_CamFromR0.Rux =  1.0f;
	m_CamFromR0.Ruy =  0.0f;
	m_CamFromR0.Ruz =  0.0f;
	m_CamFromR0.Rvx =  0.0f;
	m_CamFromR0.Rvy =  0.0f;
	m_CamFromR0.Rvz =  1.0f;
	m_CamFromR0.Rwx =  0.0f;
	m_CamFromR0.Rwy = -1.0f;
	m_CamFromR0.Rwz =  0.0f;
	m_CamFromR0.Tx  =  0.0f;
	m_CamFromR0.Ty  =  7.0f;
	m_CamFromR0.Tz  =  1.8f;

	const float tab[3][4] = {
		m_Focal, 0.0f,    0.0f,  0.0f,
		0.0f,    m_Focal, 0.0f,  0.0f,
		0.0f,    0.0f,    1.0f,  0.0f,
	};

	Matrix::StaticMatrix::Matrix<float, 3, 4> Projection(tab);

	Transform2D CamFromImg(90.0f, 60.0f, 180.0f);

	while (pacemaker.Wait())
	{		
		Clear();

		Transform3D R0FromCAM = m_CamFromR0.mat.Invert();

		STARTCHRONO;

		for (uint i = 0; i < 2; i++)
		{
			const Model3D& model = models[i];

			Transform3D ObjFromCam = R0FromCAM.mat * ObjsFromR0[i].mat;

			Matrix::StaticMatrix::Matrix<float, 3, 4> _Proj = CamFromImg.mat * Projection * ObjFromCam.mat;

			for (const Model3D::Face& face : model.Faces())
			{
				static HVector3D nw1(-m_Focal,  0.0f,    m_Width / 2);
				static HVector3D nw2( m_Focal,  0.0f,    m_Width / 2);
				static HVector3D nh1( 0.0f,     m_Focal, m_Height / 2);
				static HVector3D nh2( 0.0f,    -m_Focal, m_Height / 2);

				HVector3D _v1    = ObjFromCam.mat * model.Vertices()[face.v1].mat;
				HVector3D _v2    = ObjFromCam.mat * model.Vertices()[face.v2].mat;
				HVector3D _v3    = ObjFromCam.mat * model.Vertices()[face.v3].mat;
				HVector3D _nface = ObjFromCam.mat * model.Normals()[face.vn1].mat;

				if ((_v1 | _nface) > 0.0f)
					continue;

				HVector2D _pt1 = _Proj * model.Vertices()[face.v1].mat;
				HVector2D _pt2 = _Proj * model.Vertices()[face.v2].mat;
				HVector2D _pt3 = _Proj * model.Vertices()[face.v3].mat;

				if(IsInFOV(_v1) && IsInFOV(_v2))
					DrawLine(_pt1.PX(), _pt1.PY(), _pt2.PX(), _pt2.PY());

				if (IsInFOV(_v2) && IsInFOV(_v3))
					DrawLine(_pt2.PX(), _pt2.PY(), _pt3.PX(), _pt3.PY());

				if (IsInFOV(_v3) && IsInFOV(_v1))
					DrawLine(_pt3.PX(), _pt3.PY(), _pt1.PX(), _pt1.PY());
			}
		}

		ENDCHRONO;

		HeartBeat();

		std::stringstream sstr;

		sstr << "Ellapsed time : " << (float)ellapsed_micros / 1000.0f << " ms";

		DisplayMessage(sstr.str());

		Render();

		a += aspeed * dt;

		ObjsFromR0[1].Rux =  std::cos(TORAD(a));
		ObjsFromR0[1].Ruy =  std::sin(TORAD(a));
		ObjsFromR0[1].Rvx = -std::sin(TORAD(a));
		ObjsFromR0[1].Rvy =  std::cos(TORAD(a));
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

bool Console::IsInFOV(const HVector3D& vec) const
{
	// Assuming that the point coordinates are relative to the camera POV
	static const float zmin = 0.1f;
	
	return ((vec.z > zmin) &&
		    (vec.z > (2 * m_Focal * std::abs(vec.x)) / m_Width) &&
		    (vec.z > (2 * m_Focal * std::abs(vec.y)) / m_Height));
}

void Console::DrawPoint(float x, float y, char c)
{
	if (x < 0 || x >= m_Width)
		return;

	if (y < 0 || y >= m_Height)
		return;

	m_Screen[(INT32)x + (INT32)y * m_Width] = c;
}

void Console::DrawLine(float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;

	if (dx == 0 && dy == 0)
	{
		DrawPoint(x1, y1);
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

		DrawPoint(X, Y);
	}

	DrawPoint(x1, y1);
	DrawPoint(x2, y2);
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