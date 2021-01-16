#include <iostream>

//#include "TestApps/AppTest01.h"
#include "Misc/AppTestEvent.h"
//#include "Misc/AppTestBasicPaint.h"
//#include "Misc/RTAppTest.h"

using namespace LCN;

int main(int argc, char** argv)
{
	try
	{
		Application::Get<LCNApp>().Run();

		return EXIT_SUCCESS;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Critical error occured : " << e.what() << std::endl;

		std::cin.get();

		return EXIT_FAILURE;
	}
}