#include <iostream>

#include "Core/Application.h"

int main(int argc, char** argv)
{
	Application& app = Application::Get();

	app.Run();

	//std::cin.get();
}