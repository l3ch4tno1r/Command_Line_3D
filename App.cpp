#ifdef MAIN

#include <iostream>
#include <thread>

#include <Console.h>

using namespace std;

int main()
{
	Console& console = Console::Get();

	while (true)
	{
		this_thread::sleep_for(16ms);

		if (GetAsyncKeyState((unsigned short)27) & 0x8000)
			break;

		console.Clear();

		//console.DrawLine(10.1f, 61.2f, 80.4f, 15.3f);
		//console.DrawLine(10.0f, 61.0f, 80.0f, 61.0f);
		//console.DrawLine(0.0f, 0.0f, 3.0f, 4.0f);

		console.DrawLine(50, 50, 60, 50);
		console.DrawLine(50, 50, 60, 60);
		console.DrawLine(50, 50, 50, 60);
		console.DrawLine(50, 50, 40, 60);
		console.DrawLine(50, 50, 40, 50);
		console.DrawLine(50, 50, 40, 40);
		console.DrawLine(50, 50, 50, 40);
		console.DrawLine(50, 50, 60, 40);

		console.Render();
	}

	return 0;
}

#endif // MAIN