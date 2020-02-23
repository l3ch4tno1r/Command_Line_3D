#ifdef MAIN

#include <iostream>
#include <thread>

#include "Devices\PaceMaker.h"
#include "Devices\Console.h"

#include "Geometry\Geometry2D\Transform2D.h"
#include "Geometry\Geometry2D\Vector2D.h"
#include "Geometry\Geometry3D\Transform3D.h"
#include "Geometry\Geometry3D\Vector3D.h"

#include "Utilities\TimeMeasurement.h"
#include "Utilities\Angles.h"

using namespace std;

int main()
{
	PaceMaker& pacemaker = PaceMaker::Get();
	Console&   console   = Console::Get();

	float aspeed = 30.0f;			// 1 tour / min
	float dt     = 16.0f / 1000.0f;	// Delta de temps
	float a      = 0.0f;			// Angle

	float radius = 35;

	const chrono::milliseconds interval = 16ms;

	chrono::high_resolution_clock::time_point start, next;

	// Model
	Vector3D pt1(-0.5f, -0.5f, 0.0f);
	Vector3D pt2( 0.5f, -0.5f, 0.0f);
	Vector3D pt3( 0.5f,  0.5f, 0.0f);
	Vector3D pt4(-0.5f,  0.5f, 0.0f);
	Vector3D pt5(-0.5f, -0.5f, 1.0f);
	Vector3D pt6( 0.5f, -0.5f, 1.0f);
	Vector3D pt7( 0.5f,  0.5f, 1.0f);
	Vector3D pt8(-0.5f,  0.5f, 1.0f);

	Transform3D CubeFromR0;

	CubeFromR0.Tx = 0.5f;
	CubeFromR0.Ty = 0.5f;

	// Set up Camera transform
	Vector3D tempz = { -2.0f, -3.0f, -1.5f };
	Vector3D tempx = Vector3D::Z() ^ tempz;
	Vector3D tempy = tempz ^ tempx;

	float normx = tempx.mat.Norm();
	float normy = tempy.mat.Norm();
	float normz = tempz.mat.Norm();

	Transform3D CamFromR0;

	CamFromR0.Rux = tempx.x / normx;
	CamFromR0.Ruy = tempx.y / normx;
	CamFromR0.Ruz = tempx.z / normx;
	CamFromR0.Rvx = tempy.x / normy;
	CamFromR0.Rvy = tempy.y / normy;
	CamFromR0.Rvz = tempy.z / normy;
	CamFromR0.Rwx = tempz.x / normz;
	CamFromR0.Rwy = tempz.y / normz;
	CamFromR0.Rwz = tempz.z / normz;
	CamFromR0.Tx  = 2.0f;
	CamFromR0.Ty  = 3.0f;
	CamFromR0.Tz  = 1.8f;

	float focal = 180.0f;

	const float tab[3][4] = {
		focal, 0.0f,  0.0f,  0.0f,
		0.0f,  focal, 0.0f,  0.0f,
		0.0f,  0.0f,  1.0f,  0.0f,
	};

	Matrix::StaticMatrix::Matrix<float, 3, 4> Projection(tab);

	Transform2D CamFromImg(90.0f, 60.0f, 180.0f);

	while (true)
	{
		pacemaker.Wait();

		if (GetAsyncKeyState((unsigned short)27) & 0x8000)
		{
			pacemaker.Stop();
			break;
		}

		console.Clear();

		Matrix::StaticMatrix::Matrix<float, 3, 4> _Proj = CamFromImg.mat * Projection * CamFromR0.mat.Invert() * CubeFromR0.mat;

		Vector2D _pt1 = _Proj * pt1.mat;
		Vector2D _pt2 = _Proj * pt2.mat;
		Vector2D _pt3 = _Proj * pt3.mat;
		Vector2D _pt4 = _Proj * pt4.mat;
		Vector2D _pt5 = _Proj * pt5.mat;
		Vector2D _pt6 = _Proj * pt6.mat;
		Vector2D _pt7 = _Proj * pt7.mat;
		Vector2D _pt8 = _Proj * pt8.mat;

		console.DrawLine(_pt1.PX(), _pt1.PY(), _pt2.PX(), _pt2.PY());
		console.DrawLine(_pt2.PX(), _pt2.PY(), _pt3.PX(), _pt3.PY());
		console.DrawLine(_pt3.PX(), _pt3.PY(), _pt1.PX(), _pt1.PY());
		console.DrawLine(_pt1.PX(), _pt1.PY(), _pt4.PX(), _pt4.PY());
		console.DrawLine(_pt4.PX(), _pt4.PY(), _pt3.PX(), _pt3.PY());

		console.DrawLine(_pt1.PX(), _pt1.PY(), _pt5.PX(), _pt5.PY());
		console.DrawLine(_pt5.PX(), _pt5.PY(), _pt6.PX(), _pt6.PY());
		console.DrawLine(_pt6.PX(), _pt6.PY(), _pt1.PX(), _pt1.PY());
		//console.DrawLine(_pt1.PX(), _pt1.PY(), _pt4.PX(), _pt4.PY());
		//console.DrawLine(_pt4.PX(), _pt4.PY(), _pt3.PX(), _pt3.PY());

		console.DrawLine(_pt4.PX(), _pt4.PY(), _pt5.PX(), _pt5.PY());

		console.DrawLine(_pt6.PX(), _pt6.PY(), _pt2.PX(), _pt2.PY());
		console.DrawLine(_pt2.PX(), _pt2.PY(), _pt7.PX(), _pt7.PY());
		console.DrawLine(_pt7.PX(), _pt7.PY(), _pt6.PX(), _pt6.PY());
		console.DrawLine(_pt2.PX(), _pt2.PY(), _pt3.PX(), _pt3.PY());
		console.DrawLine(_pt3.PX(), _pt3.PY(), _pt7.PX(), _pt7.PY());

		console.DrawLine(_pt3.PX(), _pt3.PY(), _pt8.PX(), _pt8.PY());
		console.DrawLine(_pt8.PX(), _pt8.PY(), _pt7.PX(), _pt7.PY());
		console.DrawLine(_pt3.PX(), _pt3.PY(), _pt4.PX(), _pt4.PY());
		console.DrawLine(_pt4.PX(), _pt4.PY(), _pt8.PX(), _pt8.PY());
		console.DrawLine(_pt6.PX(), _pt6.PY(), _pt8.PX(), _pt8.PY());

		console.DrawLine(_pt8.PX(), _pt8.PY(), _pt5.PX(), _pt5.PY());
		console.DrawLine(_pt5.PX(), _pt5.PY(), _pt6.PX(), _pt6.PY());

		console.HeartBeat();

		console.Render();

		a += aspeed * dt;

		CubeFromR0.Rux =  std::cos(TORAD(a));
		CubeFromR0.Ruy =  std::sin(TORAD(a));
		CubeFromR0.Rvx = -std::sin(TORAD(a));
		CubeFromR0.Rvy =  std::cos(TORAD(a));
	}

	return 0;
}

#endif // MAIN