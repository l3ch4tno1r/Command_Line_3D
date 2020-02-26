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

	float camspeed = 3.0f;
	float dt       = 16.0f / 1000.0f;

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
	}

	return 0;
}

#endif // MAIN