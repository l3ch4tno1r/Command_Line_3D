#pragma once

#include <Console3D/Source/Core/Application.h>
#include <Console3D/Source/RessourceManagement/Texture.h>
#include <Console3D/Source/Scene/Scene.h>

namespace LCN
{
	class AppTestDisplayImage : public Application
	{
	private:
		template<class F>
		using SignalApp = Signal<AppTestDisplayImage, F>;

		void Run() override;

	private: // Signals
		SignalApp<void()> SignalStartup;
		SignalApp<void()> SignalQuit;

	private:

	};
}

using LCNApp = LCN::AppTestDisplayImage;