#include <iostream>

#include "AppTestDisplayImage.h"

#include <Console3D/Source/Core/Console.h>
#include <Console3D/Source/Core/ConsoleInput.h>
#include <Console3D/Source/Scene/StdComponent.h>

#include <LCN_Math/Source/Utilities/Angles.h>

namespace LCN
{
	/////////////////////////////
	//-- AppTestDisplayImage --//
	/////////////////////////////

	AppTestDisplayImage::AppTestDisplayImage() :
		SLOT_INIT(SlotStartup,  AppTestDisplayImage::Startup),
		SLOT_INIT(SlotOnUpdate, AppTestDisplayImage::OnUpdate)
	{
		Connect(this->SignalStartup, this->SlotStartup);
	}

	void AppTestDisplayImage::Startup()
	{
		m_Camera  = m_Scene.CreateEntity();
		m_Sprite1 = m_Scene.CreateEntity();
		m_Sprite2 = m_Scene.CreateEntity();

		// Sprite 1
		Component::TextureComponent& textureCmp1 = m_Sprite1.Add<Component::TextureComponent>();

		textureCmp1.Texture.Load("Ressource/Le_Chat_Noir_Photo_Alpha.png");

		ASSERT(textureCmp1.Texture);

		size_t texturew1 = textureCmp1.Texture.Width();
		size_t textureh1 = textureCmp1.Texture.Height();

		m_Sprite1.Add<Component::Sprite2DComponent>(texturew1, textureh1);

		// Sprite 2
		Component::TextureComponent& textureCmp2 = m_Sprite2.Add<Component::TextureComponent>();

		textureCmp2.Texture.Load("Ressource/Wooden_Medium.jpg");

		ASSERT(textureCmp2.Texture);

		size_t texturew2 = textureCmp2.Texture.Width();
		size_t textureh2 = textureCmp2.Texture.Height();

		m_Sprite2.Add<Component::Sprite2DComponent>(texturew2, textureh2);

		//Transform2Df& transform2 = m_Sprite2.Get<Transform2DComponent>().Transform;
		//
		//transform2.TranslationBlock() = { 100.0f, 0.0f };
		//transform2.RotationBlock() = {
		//	std::cos(TORAD(20.0f)), -std::sin(TORAD(20.0f)),
		//	std::sin(TORAD(20.0f)),  std::cos(TORAD(20.0f))
		//};

		Core::Console& console = Core::Console::Get();

		//console.ConstructConsole(150, 100, 8, 8);
		console.ConstructConsole(300, 200, 4, 4);

		m_Camera.Add<Component::Camera2DComponent>(console.Width(), console.Height());

		m_Controller.Bind(m_Camera);

		Connect(m_Controller.SignlaUpdate, this->SlotOnUpdate);

		m_Scene.Render2D(m_Camera);
	}

	void AppTestDisplayImage::OnUpdate()
	{
		m_Scene.Render2D(m_Camera);
	}

	Core::Application::AppPointer Core::Application::CreateApplication()
	{
		return std::make_unique<AppTestDisplayImage>();
	}
}