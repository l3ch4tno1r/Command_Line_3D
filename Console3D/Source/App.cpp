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

		if (GetAsyncKeyState((unsigned short)27) & 0x8000)
		{
			pacemaker.Stop();
			break;
		}

		if (GetAsyncKeyState((unsigned short)'Z') & 0x8000)
		{
			camfromr0.Tx += camspeed * camfromr0.Rwx * dt;
			camfromr0.Ty += camspeed * camfromr0.Rwy * dt;
		}

		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			camfromr0.Tx -= camspeed * camfromr0.Rwx * dt;
			camfromr0.Ty -= camspeed * camfromr0.Rwy * dt;
		}

		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
		{
			camfromr0.Tx += camspeed * camfromr0.Rux * dt;
			camfromr0.Ty += camspeed * camfromr0.Ruy * dt;
		}

		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			camfromr0.Tx -= camspeed * camfromr0.Rux * dt;
			camfromr0.Ty -= camspeed * camfromr0.Ruy * dt;
		}

		if (GetAsyncKeyState(VK_UP))
		{
			static Transform3D RotX;
			float da = -anglespeed * dt;

			RotX.Rvy =  std::cos(TORAD(da));
			RotX.Rvz =  std::sin(TORAD(da));
			RotX.Rwy = -std::sin(TORAD(da));
			RotX.Rwz =  std::cos(TORAD(da));

			camfromr0.mat = camfromr0.mat * RotX.mat;
		}

		if (GetAsyncKeyState(VK_DOWN))
		{
			static Transform3D RotX;
			float da = anglespeed * dt;

			RotX.Rvy =  std::cos(TORAD(da));
			RotX.Rvz =  std::sin(TORAD(da));
			RotX.Rwy = -std::sin(TORAD(da));
			RotX.Rwz =  std::cos(TORAD(da));

			camfromr0.mat = camfromr0.mat * RotX.mat;
		}

		if (GetAsyncKeyState(VK_LEFT))
		{
			static Transform3D RotY;
			float da = -anglespeed * dt;

			RotY.Rux =  std::cos(TORAD(da));
			RotY.Ruz =  std::sin(TORAD(da));
			RotY.Rwx = -std::sin(TORAD(da));
			RotY.Rwz =  std::cos(TORAD(da));

			camfromr0.mat = camfromr0.mat * RotY.mat;
		}
		
		if (GetAsyncKeyState(VK_RIGHT))
		{
			static Transform3D RotY;
			float da = anglespeed * dt;

			RotY.Rux =  std::cos(TORAD(da));
			RotY.Ruz =  std::sin(TORAD(da));
			RotY.Rwx = -std::sin(TORAD(da));
			RotY.Rwz =  std::cos(TORAD(da));

			camfromr0.mat = camfromr0.mat * RotY.mat;
		}
	}

	return 0;
}

#endif // MAIN