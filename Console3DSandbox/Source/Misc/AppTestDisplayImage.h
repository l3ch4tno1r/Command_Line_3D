#pragma once

#include "EventApplication.h"
#include "Texture.h"
#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "Camera2DController.h"

#include "Geometry/Geometry.h"

namespace LCN
{
	/////////////////////////////
	//-- AppTestDisplayImage --//
	/////////////////////////////

	class AppTestDisplayImage : public Core::EventApplication
	{
	public:
		AppTestDisplayImage();

	private: // Slots
		SLOT(AppTestDisplayImage, Startup);
		SLOT(AppTestDisplayImage, OnUpdate);

	private:
		Scene  m_Scene;
		Entity m_Camera;
		Entity m_Sprite1;
		Entity m_Sprite2;

		Controller::Camera2DController m_Controller;
	};
}