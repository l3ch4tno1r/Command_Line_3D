#pragma once

#include <Console3D/Source/Core/RTApplication.h>

#include <Console3D/Source/Scene/Scene.h>
#include <Console3D/Source/Scene/Entity.h>

#include <Console3D/Source/Controller/FPSCameraController.h>

namespace LCN
{
	class RTAppTest3D : public Core::RTApplication
	{
	public:
		RTAppTest3D();

	private: // Slots
		SLOT(RTAppTest3D, OnStartup);
		SLOT(RTAppTest3D, OnUpdate, float);
		SLOT(RTAppTest3D, OnRender);
		SLOT(RTAppTest3D, OnQuit);

	private:
		Scene m_Scene;

		Entity m_Camera;
		Entity m_Floor;
		Entity m_Cube;

		Controller::FPSCameraController m_Controller;
	};
}