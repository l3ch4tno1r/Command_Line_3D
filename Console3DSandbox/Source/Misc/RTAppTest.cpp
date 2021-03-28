#include "RTAppTest.h"

#include <Console3D/Source/Core/Console.h>
#include <Console3D/Source/Rendering/Renderer2D.h>
#include <Console3D/Source/Scene/StdComponent.h>

#include <Utilities/Source/Instrumentor.h>

#include <LCN_Math/Source/Utilities/Angles.h>

#include <iostream>
#include <sstream>

namespace LCN
{
	RTAppTest::RTAppTest()
	{
		Connect(this->SignalStartup, this->SlotOnStartup);
		Connect(this->SignalUpdate,  this->SlotOnUpdate);
		Connect(this->SignalRender,  this->SlotOnRender);
		Connect(this->SignalQuit,    this->SlotOnQuit);
	}

	void RTAppTest::OnStartup()
	{
		auto& console = Core::Console::Get();

		console.ConstructConsole(150, 100, 8, 8);

		// Sprite 1
		m_Sprite1 = m_Scene.CreateEntity();

		auto& texture1 = m_Sprite1.Add<Component::TextureCmp>();

		texture1.Texture.Load("Ressource/Le_Chat_Noir_Photo_Alpha.png");

		ASSERT(textureCmp1.Texture);

		size_t texturew1 = texture1.Texture.Width();
		size_t textureh1 = texture1.Texture.Height();

		m_Sprite1.Add<Component::Sprite2DCmp>(texturew1, textureh1);
		m_Sprite1.Add<Component::AnimationCmp>();

		// Sprite 2
		m_Sprite2 = m_Scene.CreateEntity();

		auto& texture2 = m_Sprite2.Add<Component::TextureCmp>();

		texture2.Texture.Load("Ressource/Wooden_Medium.jpg");

		ASSERT(textureCmp1.Texture);

		size_t texturew2 = texture2.Texture.Width();
		size_t textureh2 = texture2.Texture.Height();

		m_Sprite2.Add<Component::Sprite2DCmp>(texturew2, textureh2);

		// Camera
		m_Camera = m_Scene.CreateEntity();

		m_Camera.Add<Component::Camera2DCmp>(console.Width(), console.Height());

		m_Controller.Bind(m_Camera);

		Instrumentor::Get().BeginSession("RTAppTest");
	}

	void RTAppTest::OnUpdate(float delta)
	{
		//PROFILE_FUNC();

		const float aspeed = 36.0f;			     // 1 tour / 10s
		const float dt     = delta / 1000.0f;    // Delta de temps
		const float da     = TORAD(aspeed * dt); // Angle

		auto view = m_Scene.Registry().view<Component::Transform2DCmp, Component::AnimationCmp>();

		view.each([=](
			Component::Transform2DCmp& transformCmp,
			Component::AnimationCmp&   animationCmp)
			{
				static float a = 0.0f;
				a += da;
		
				auto rotation = transformCmp.Transform.RotationBlock();
		
				rotation(0, 0) =  std::cos(a);
				rotation(0, 1) = -std::sin(a);
				rotation(1, 0) =  std::sin(a);
				rotation(1, 1) =  std::cos(a);
			});
	}

	void RTAppTest::OnRender()
	{
		//PROFILE_FUNC();

		Render::Renderer2D::RenderSequential(m_Scene, m_Camera);
		Render::Renderer2D::Submit();
	}

	void RTAppTest::OnQuit()
	{
		Instrumentor::Get().EndSession();
	}

	CREATE_APP(RTAppTest);
}
