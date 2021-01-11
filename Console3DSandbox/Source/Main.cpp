#include <iostream>

#include "TestApps/AppTest01.h"

int main(int argc, char** argv)
{
	try
	{
		LCNApp::Get().Run();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cout << "Critical error occured : " << e.what() << std::endl;

		std::cin.get();

		return EXIT_FAILURE;
	}
}