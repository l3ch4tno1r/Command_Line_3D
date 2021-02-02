#pragma once

#include <Console3D/Source/Core/Application.h>
#include <Console3D/Source/RessourceManagement/Texture.h>
#include <Console3D/Source/Scene/Scene.h>
#include <Console3D/Source/Scene/Entity.h>
#include <Console3D/Source/Rendering/Camera2D.h>
#include <Console3D/Source/Controller/Camera2DController.h>

#include <LCN_Math/Source/Geometry/Geometry.h>

namespace LCN
{
	/////////////////////////////
	//-- AppTestDisplayImage --//
	/////////////////////////////

	class AppTestDisplayImage : public Application
	{
	public:
		AppTestDisplayImage();

		template<class F>
		using SignalApp = Signal<AppTestDisplayImage, F>;

		void Run() override;

	private: // Slots
		SLOT(AppTestDisplayImage, Startup);
		SLOT(AppTestDisplayImage, OnUpdate);

	private: // Signals
		SignalApp<void()> SignalStartup;
		SignalApp<void()> SignalQuit;

	private:
		Scene  m_Scene;
		Entity m_Camera;
		Entity m_Sprite1;
		Entity m_Sprite2;

		Camera2DController m_Controller;
	};
}

using LCNApp = LCN::AppTestDisplayImage;