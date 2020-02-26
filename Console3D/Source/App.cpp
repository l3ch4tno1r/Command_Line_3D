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

	while (true)
	{
		pacemaker.Wait();

		if (GetAsyncKeyState((unsigned short)27) & 0x8000)
		{
			pacemaker.Stop();
			break;
		}		
	}

	return 0;
}

#endif // MAIN