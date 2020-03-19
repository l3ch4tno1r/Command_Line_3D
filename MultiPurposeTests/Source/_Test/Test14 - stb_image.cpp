#ifdef TEST14

#include <iostream>
#include <string>

#include "External\stb_image\stb_image.h" 

int main()
{
	/*
	unsigned int size      = 71;
	const char*  grayscale = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
	*/
	unsigned int size      = 10;
	const char*  grayscale = " .:-=+*#%@";

	std::string filepath("Ressources/Le_Chat_Noir_Photo_Medium.png");

	unsigned char* localbuffer;
	int width;
	int height;
	int BPP;

	localbuffer = stbi_load(filepath.c_str(), &width, &height, &BPP, STBI_grey);

	if (stbi_failure_reason())
		std::cout << stbi_failure_reason() << std::endl;

	if (!localbuffer)
		std::cout << "Image not found." << std::endl;
	else
	{
		std::cout << width << ", " << height << ", " << BPP << std::endl;

		for (unsigned int j = 0; j < height; j++)
		{
			for (unsigned int i = 0; i < width; i++)
			{
				unsigned char pxl = ((size - 1) * localbuffer[i + j * width]) / 255;

				std::cout << grayscale[pxl];
			}
	
			std::cout << std::endl;
		}

		stbi_image_free(localbuffer);
	}

	std::cin.get();
}

#endif // TEST14
