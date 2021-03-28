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
		//console.ConstructConsole(300, 200, 4, 4);

		// Sprite 0
		m_SpriteToast = m_Scene.CreateEntity();

		auto& texture0 = m_SpriteToast.Add<Component::TextureCmp>();

		texture0.Texture.Load("Ressource/Textures/Toast01_Small.png");

		ASSERT(texture0.Texture);

		m_SpriteToast.Add<Component::Sprite2DCmp>(
			2 * texture0.Texture.Width() / 3,
			2 * texture0.Texture.Height() / 3);

		// Sprite 1
		m_SpriteLCN = m_Scene.CreateEntity();

		auto& texture1 = m_SpriteLCN.Add<Component::TextureCmp>();

		texture1.Texture.Load("Ressource/Textures/Le_Chat_Noir_Photo_Alpha.png");

		ASSERT(texture1.Texture);

		m_SpriteLCN.Add<Component::Sprite2DCmp>(
			texture1.Texture.Width(),
			texture1.Texture.Height());

		m_SpriteLCN.Add<Component::AnimationCmp>();

		// Attach Sprite 0 to Sprite 1
		m_SpriteToast.Add<Component::ParentCmp>(m_SpriteLCN);
		auto& E1ToE0 = m_SpriteToast.Add<Component::OffsetCmp>().Transform;

		E1ToE0.TranslationBlock() = { 20.0f, -40.0f };

		// Sprite 2
		m_SpritePlank = m_Scene.CreateEntity();

		auto& texture2 = m_SpritePlank.Add<Component::TextureCmp>();

		texture2.Texture.Load("Ressource/Textures/Wooden_Medium.jpg");

		ASSERT(texture2.Texture);

		m_SpritePlank.Add<Component::Sprite2DCmp>(
			texture2.Texture.Width(),
			texture2.Texture.Height());

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

		// Run animation
		auto groupAnimated = m_Scene.Registry().group<Component::AnimationCmp>(entt::get<Component::Transform2DCmp>);

		groupAnimated.each([=](
			Component::AnimationCmp&   animationCmp,
			Component::Transform2DCmp& transformCmp)
			{
				static float a = 0.0f;
				a += da;
		
				auto rotation = transformCmp.Transform.RotationBlock();
		
				rotation(0, 0) =  std::cos(a);
				rotation(0, 1) = -std::sin(a);
				rotation(1, 0) =  std::sin(a);
				rotation(1, 1) =  std::cos(a);
			});

		// Update entities with parents
		auto groupWithParents = m_Scene.Registry().group<Component::ParentCmp, Component::OffsetCmp>(entt::get<Component::Transform2DCmp>);

		groupWithParents.each([](
			Component::ParentCmp&      parentCmp,
			Component::OffsetCmp&      offsetCmp,
			Component::Transform2DCmp& transformCmp)
			{
				auto& R0ToParent = parentCmp.Parent.Get<Component::Transform2DCmp>().Transform;

				transformCmp.Transform = R0ToParent * offsetCmp.Transform;
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
