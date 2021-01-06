#include <iostream>

#ifdef DEBUG
#define TEST
#endif // DEBUG


#ifndef TEST
#include "Core/Application.h"
#else
#include "Core/_Tests/AppTest.h"

using Application = AppTest;
#endif // TEST


int main(int argc, char** argv)
{
	try
	{
		Application::Get().Run();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error occured : " << e.what() << std::endl;

		std::cin.get();

		return EXIT_FAILURE;
	}
}