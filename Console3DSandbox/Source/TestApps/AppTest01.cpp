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
		console.FillRectangle(0 * squarewidth, squarewidth * i, 1 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_RED);
		console.FillRectangle(1 * squarewidth, squarewidth * i, 2 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_YELLOW);
		console.FillRectangle(2 * squarewidth, squarewidth * i, 3 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_GREEN);
		console.FillRectangle(3 * squarewidth, squarewidth * i, 4 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_CYAN);
		console.FillRectangle(4 * squarewidth, squarewidth * i, 5 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_BLUE);
		console.FillRectangle(5 * squarewidth, squarewidth * i, 6 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_MAGENTA);
	}

	for (int i = 0; i < 10; ++i)
	{
		console.FillRectangle(7 * squarewidth, squarewidth * i, 8 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_RED | COLOUR::BG_BLACK);
		console.FillRectangle(8 * squarewidth, squarewidth * i, 9 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_RED | COLOUR::BG_DARK_GREY);
		console.FillRectangle(9 * squarewidth, squarewidth * i, 10 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_RED | COLOUR::BG_GREY);
		console.FillRectangle(10 * squarewidth, squarewidth * i, 11 * squarewidth, squarewidth * (i + 1), value[i], COLOUR::FG_RED | COLOUR::BG_WHITE);
	}

	console.Render();

	std::cin.get();
}