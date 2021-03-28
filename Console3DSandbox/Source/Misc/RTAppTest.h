#pragma once

#include <Console3D/Source/Core/RTApplication.h>

#include <Console3D/Source/Scene/Scene.h>
#include <Console3D/Source/Scene/Entity.h>

#include <Console3D/Source/Controller/Camera2DController.h>

namespace LCN
{
	class RTAppTest : public Core::RTApplication
	{
	public:
		RTAppTest();

	public: // Slots
		SLOT(RTAppTest, OnStartup);
		SLOT(RTAppTest, OnUpdate, float);
		SLOT(RTAppTest, OnRender);
		SLOT(RTAppTest, OnQuit);

	private:
		Scene m_Scene;

		Entity m_Camera;
		Entity m_SpriteToast;
		Entity m_SpriteLCN;
		Entity m_SpritePlank;

		Controller::Camera2DController m_Controller;
	};
}

using LCNApp = LCN::RTAppTest;