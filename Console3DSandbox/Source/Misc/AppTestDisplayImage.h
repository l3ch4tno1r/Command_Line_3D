#pragma once

#include <Console3D/Source/Core/Application.h>
#include <Console3D/Source/RessourceManagement/Texture.h>

namespace LCN
{
	class AppTestDisplayImage : public Application
	{
	private:
		void Run() override;
	};
}

using LCNApp = LCN::AppTestDisplayImage;