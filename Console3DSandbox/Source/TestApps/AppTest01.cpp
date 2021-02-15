#include "AppTest01.h"

void LCN::AppTest::Run()
{
	Console& console = Console::Get();

	console.ConstructConsole(150, 100, 8, 8);

	console.Clear();

	const char* value = " -.:*+=%#@";
	const size_t squarewidth = 9;

	for (int i = 0; i < 10; i++)
	{
		console.FillRectangle(0 * squarewidth, squarewidth * i, 1 * squarewidth, squarewidth * (i + 1));
		console.FillRectangle(1 * squarewidth, squarewidth * i, 2 * squarewidth, squarewidth * (i + 1));
		console.FillRectangle(2 * squarewidth, squarewidth * i, 3 * squarewidth, squarewidth * (i + 1));
		console.FillRectangle(3 * squarewidth, squarewidth * i, 4 * squarewidth, squarewidth * (i + 1));
		console.FillRectangle(4 * squarewidth, squarewidth * i, 5 * squarewidth, squarewidth * (i + 1));
		console.FillRectangle(5 * squarewidth, squarewidth * i, 6 * squarewidth, squarewidth * (i + 1));
	}

	for (int i = 0; i < 10; ++i)
	{
		console.FillRectangle(7 *  squarewidth, squarewidth * i,  8 * squarewidth, squarewidth * (i + 1));
		console.FillRectangle(8 *  squarewidth, squarewidth * i,  9 * squarewidth, squarewidth * (i + 1));
		console.FillRectangle(9 *  squarewidth, squarewidth * i, 10 * squarewidth, squarewidth * (i + 1));
		console.FillRectangle(10 * squarewidth, squarewidth * i, 11 * squarewidth, squarewidth * (i + 1));
	}

	console.Render();

	std::cin.get();
}