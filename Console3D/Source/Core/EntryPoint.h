#pragma once

#include <windows.h>

using namespace LCN::Core;

int main(int argc, char** argv)
{
	timeBeginPeriod(1);

	try
	{
		Application::Get().Run();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Critical error occured : " << e.what() << std::endl;

		std::cin.get();

		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "Unknown error occured" << std::endl;

		std::cin.get();

		return EXIT_FAILURE;
	}

	timeEndPeriod(1);
}