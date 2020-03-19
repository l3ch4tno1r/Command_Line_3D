#ifdef TEST14

#include <iostream>
#include <string>

#include "External\stb_image\stb_image.h" 

int main()
{
	//const char* grayscale = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
	const char* grayscale = " .:-=+*#%@";

	std::string filepath("Ressources/gradient.png");

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
				unsigned char pxl = (9 * localbuffer[i + j * width]) / 255;

				std::cout << grayscale[pxl];
			}
	
			std::cout << std::endl;
		}

		stbi_image_free(localbuffer);
	}

	std::cin.get();
}

#endif // TEST14
