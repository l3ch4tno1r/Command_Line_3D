#pragma once

using namespace LCN::Core;

int main(int argc, char** argv)
{
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
}