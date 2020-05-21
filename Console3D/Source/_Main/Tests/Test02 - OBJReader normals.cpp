#include <iostream>

#include "Graphics/OBJReader.h"

int main()
{
	Model3D model = OBJReader().ReadFile<Model3D>("Ressource/octogon_no_normals.obj", true);

	std::cin.get();
}