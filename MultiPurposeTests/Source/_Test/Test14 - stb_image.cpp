#ifdef TEST14

#include <iostream>
#include <string>

#include "External\stb_image\stb_image.h" 

int main()
{
	std::string filepath("Ressources/Test.png");

	unsigned char* localbuffer;
	int width;
	int height;
	int BPP;

	localbuffer = stbi_load(filepath.c_str(), &width, &height, &BPP, STBI_grey);

	if (stbi_failure_reason())
		std::cout << stbi_failure_reason() << std::endl;

	std::cout << width << ", " << height << ", " << BPP << std::endl;

	for (unsigned int j = 0; j < height; j++)
	{
		for (unsigned int i = 0; i < width; i++)
			std::cout << localbuffer[i + j * width];

		std::cout << std::endl;
	}

	std::cin.get();

	stbi_image_free(localbuffer);
}

#endif // TEST14
