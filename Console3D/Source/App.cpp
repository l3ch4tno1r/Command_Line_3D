#ifdef MAIN

#include <iostream>
#include <thread>

#include "Devices\PaceMaker.h"
#include "Devices\Console.h"

#include "Geometry\Geometry2D\Transform2D.h"
#include "Geometry\Geometry2D\Vector2D.h"
#include "Geometry\Geometry3D\Transform3D.h"
#include "Geometry\Geometry3D\Vector3D.h"

#include "Graphics\OBJReader.h"

#include "Utilities\TimeMeasurement.h"
#include "Utilities\Angles.h"

using namespace std;

int main()
{
	PaceMaker& pacemaker = PaceMaker::Get();
	Console&   console   = Console::Get();	

	Transform3D& camfromr0 = console.CamFromR0();

	float camspeed   = 3.0f;
	float anglespeed = 32.0f;
	float dt         = 16.0f / 1000.0f;

	while (true)
	{
		pacemaker.Wait();

		static Transform3D Tr = camfromr0;
		static Transform3D RotX;
		static Transform3D RotY;

		// Translation management
		if (GetAsyncKeyState((unsigned short)27) & 0x8000)
		{
			pacemaker.Stop();
			break;
		}

		if (GetAsyncKeyState((unsigned short)'Z') & 0x8000)
		{
			Tr.Tx += camspeed * camfromr0.Rwx * dt;
			Tr.Ty += camspeed * camfromr0.Rwy * dt;
		}

		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			Tr.Tx -= camspeed * camfromr0.Rwx * dt;
			Tr.Ty -= camspeed * camfromr0.Rwy * dt;
		}

		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
		{
			Tr.Tx += camspeed * camfromr0.Rux * dt;
			Tr.Ty += camspeed * camfromr0.Ruy * dt;
		}

		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			Tr.Tx -= camspeed * camfromr0.Rux * dt;
			Tr.Ty -= camspeed * camfromr0.Ruy * dt;
		}

		// Rotation management
		static float ax = 0.0f;
		static float ay = 0.0f;

		if (GetAsyncKeyState(VK_UP))
		{
			ax -= anglespeed * dt;

			RotX.Rvy =  std::cos(TORAD(ax));
			RotX.Rvz =  std::sin(TORAD(ax));
			RotX.Rwy = -std::sin(TORAD(ax));
			RotX.Rwz =  std::cos(TORAD(ax));
		}

		if (GetAsyncKeyState(VK_DOWN))
		{
			ax += anglespeed * dt;

			RotX.Rvy =  std::cos(TORAD(ax));
			RotX.Rvz =  std::sin(TORAD(ax));
			RotX.Rwy = -std::sin(TORAD(ax));
			RotX.Rwz =  std::cos(TORAD(ax));
		}
		
		if (GetAsyncKeyState(VK_LEFT))
		{
			ay -= anglespeed * dt;

			RotY.Rux =  std::cos(TORAD(ay));
			RotY.Ruz =  std::sin(TORAD(ay));
			RotY.Rwx = -std::sin(TORAD(ay));
			RotY.Rwz =  std::cos(TORAD(ay));
		}

		if (GetAsyncKeyState(VK_RIGHT))
		{
			ay += anglespeed * dt;

			RotY.Rux =  std::cos(TORAD(ay));
			RotY.Ruz =  std::sin(TORAD(ay));
			RotY.Rwx = -std::sin(TORAD(ay));
			RotY.Rwz =  std::cos(TORAD(ay));
		}

		camfromr0.mat = Tr.mat * RotY.mat * RotX.mat;
	}

	return 0;
}

#endif // MAIN