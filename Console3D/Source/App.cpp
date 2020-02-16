#ifdef MAIN

#include <iostream>
#include <thread>

#include "Console.h"
#include "Matrix2D\Matrix33f.h"

using namespace std;

//using Vector2D = Matrix31f;

int main()
{
	Console& console = Console::Get();

	float aspeed = 10;				// 1 tour / min
	float dt     = 16.0f / 1000.0f;	// Delta de temps
	float a      = 0;				// Angle

	float radius = 35;

	const chrono::milliseconds interval = 16ms;

	chrono::high_resolution_clock::time_point start, next;

	// Model
	//Vector2D pt1( 20.0f,  20.0f);
	//Vector2D pt2(-20.0f,  20.0f);
	//Vector2D pt3(-20.0f, -20.0f);
	//Vector2D pt4( 20.0f, -20.0f);
	//
	//Matrix33f mat(60.0f, 40.0f, 0.0f);

	while (true)
	{
		start = chrono::high_resolution_clock::now();
		next  = start + interval;

		if (GetAsyncKeyState((unsigned short)27) & 0x8000)
			break;

		console.Clear();

		//console.DrawLine(10.1f, 61.2f, 80.4f, 15.3f);
		//console.DrawLine(10.1f, 61.2f, 30.0f, 15.3f);
		//console.DrawLine(10.1f, 61.2f, 80.4f, 70.0f);
		//console.DrawLine(0.0f, 0.0f, 3.0f, 4.0f);
		//
		//console.DrawLine(50, 50, 60, 50);
		//console.DrawLine(50, 50, 60, 60);
		//console.DrawLine(50, 50, 50, 60);
		//console.DrawLine(50, 50, 40, 60);
		//console.DrawLine(50, 50, 40, 50);
		//console.DrawLine(50, 50, 40, 40);
		//console.DrawLine(50, 50, 50, 40);
		//console.DrawLine(50, 50, 60, 40);

		//auto _pt1 = mat * pt1;
		//auto _pt2 = mat * pt2;
		//auto _pt3 = mat * pt3;
		//auto _pt4 = mat * pt4;
		//
		//console.DrawLine(_pt1(0, 0), _pt1(1, 0), _pt2(0, 0), _pt2(1, 0));
		//console.DrawLine(_pt2(0, 0), _pt2(1, 0), _pt3(0, 0), _pt3(1, 0));
		//console.DrawLine(_pt3(0, 0), _pt3(1, 0), _pt4(0, 0), _pt4(1, 0));
		//console.DrawLine(_pt4(0, 0), _pt4(1, 0), _pt1(0, 0), _pt1(1, 0));

		a += aspeed * dt;

		//mat.SetRotationAngle(a);

		console.HeartBeat();

		console.Render();

		this_thread::sleep_until(next);
	}

	return 0;
}

#endif // MAIN