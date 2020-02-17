#ifdef MAIN

#include <iostream>
#include <thread>

#include "Console.h"
#include "Matrix2D\Matrix33f.h"

using namespace std;

using Vector2D  = Vector2D;
using Transform = Transform2D;

int main()
{
	Console& console = Console::Get();

	float aspeed = 30.0f;			// 1 tour / min
	float dt     = 16.0f / 1000.0f;	// Delta de temps
	float a      = 0.0f;				// Angle

	float radius = 35;

	const chrono::milliseconds interval = 16ms;

	chrono::high_resolution_clock::time_point start, next;

	//// Model
	Vector2D pt1( 20.0f,  20.0f);
	Vector2D pt2(-20.0f,  20.0f);
	Vector2D pt3(-20.0f, -20.0f);
	Vector2D pt4( 20.0f, -20.0f);
	
	Transform transform(60.0f, 40.0f, a);

	while (true)
	{
		start = chrono::high_resolution_clock::now();
		next  = start + interval;

		if (GetAsyncKeyState((unsigned short)27) & 0x8000)
			break;

		console.Clear();

		Vector2D _pt1 = transform * pt1.mat;
		Vector2D _pt2 = transform * pt2.mat;
		Vector2D _pt3 = transform * pt3.mat;
		Vector2D _pt4 = transform * pt4.mat;
		
		console.DrawLine(_pt1.x, _pt1.y, _pt2.x, _pt2.y);
		console.DrawLine(_pt2.x, _pt2.y, _pt3.x, _pt3.y);
		console.DrawLine(_pt3.x, _pt3.y, _pt4.x, _pt4.y);
		console.DrawLine(_pt4.x, _pt4.y, _pt1.x, _pt1.y);

		a += aspeed * dt;

		transform.SetRotationAngle(a);
		//transform(0, 0) =  std::cos(TORAD(a));
		//transform(1, 0) =  std::sin(TORAD(a));
		//transform(0, 1) = -std::sin(TORAD(a));
		//transform(1, 1) =  std::cos(TORAD(a));

		console.HeartBeat();

		console.Render();

		this_thread::sleep_until(next);
	}

	return 0;
}

#endif // MAIN