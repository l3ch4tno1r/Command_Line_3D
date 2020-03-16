#ifdef TEST06

#include <iostream>

#include "Graphics\OBJReader.h"

int main()
{
	try
	{
		OBJReader reader;

		Model3D model = reader.ReadFile("Ressource/cube.obj");

		std::cout << "Done !" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception : " << e.what() << std::endl;
	}

	std::cin.get();
}

#endif // TEST06
