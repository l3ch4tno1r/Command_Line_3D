#include "RTAppTest3D.h"

#include "Console3D/Source/Rendering/ConsoleRenderer.h"
#include "Console3D/Source/Scene/StdComponent.h"

#include "Utilities/Source/ConfigManager.h"

namespace LCN
{
	RTAppTest3D::RTAppTest3D()
	{
		Connect(this->SignalStartup, this->SlotOnStartup);
		Connect(this->SignalUpdate,  this->SlotOnUpdate);
		Connect(this->SignalRender,  this->SlotOnRender);
		Connect(this->SignalQuit,    this->SlotOnQuit);
	}

	void RTAppTest3D::OnStartup()
	{
		Render::ConsolerRenderer::Init(150, 100, 8, 8);
		//Render::ConsolerRenderer::Init(300, 200, 4, 4);

		// Setup camera entity
		m_Camera = m_Scene.Create3DEntity();

		m_Camera.Add<LCN::Component::CameraCmp>(
			Render::ConsolerRenderer::Width(),
			Render::ConsolerRenderer::Height(),
			120.0f);

		auto& camTransform = m_Camera.Get<LCN::Component::Transform3DCmp>();

		camTransform.Transform = {
			1.0f, 0.0f,  0.0f,  0.0f,
			0.0f, 0.0f, -1.0f, 10.0f,
			0.0f, 1.0f,  0.0f,  1.8f,
			0.0f, 0.0f,  0.0f,  1.0f
		};

		m_Controller.Bind(m_Camera);
		m_Controller.Init();

		auto& appSettings = LCN::Utilities::ConfigManager::Get();

		m_Controller.SetGoForwardKey(appSettings["goforward"].Value<Core::Key>());
		m_Controller.SetGoBackwardKey(appSettings["gobackward"].Value<Core::Key>());
		m_Controller.SetStrafeLeftKey(appSettings["strafeleft"].Value<Core::Key>());
		m_Controller.SetStrafeRightKey(appSettings["straferight"].Value<Core::Key>());

		// Setup ifinite chessboard floor entity
		m_Floor = m_Scene.Create3DEntity();

		m_Floor.Add<Component::InfiniteChessboardCmp>(1, 1);

		// Setup globe entity
		m_Globe = m_Scene.Create3DEntity();

		m_Globe.Add<Component::SphereCmp>(2.0f);
		auto& textureCmp = m_Globe.Add<Component::TextureCmp>();

		textureCmp.Texture.Load("Ressource/Textures/Mappemonde.png");
		//textureCmp.Texture.Load("Ressource/Textures/Le_Chat_Noir_Photo_Medium.png");

		if (!textureCmp.Texture)
			throw std::exception("Pas de texture !");

		auto globeTranslation = m_Globe.Get<Component::Transform3DCmp>().Transform.TranslationBlock();

		globeTranslation = { 0.0f, 0.0f, 2.0f };

		// Setup cube entity
		m_Cube = m_Scene.Create3DEntity();

		m_Cube.Add<Component::CubeCmp>(1.0f);
		auto& diceTexture = m_Cube.Add<Component::TextureCmp>().Texture;

		diceTexture.Load("Ressource/Textures/Dice_Texture.png");

		if (!diceTexture)
			throw std::exception("Pas de texture !");

		auto& diceTransfrom = m_Cube.Get<Component::Transform3DCmp>().Transform;

		diceTransfrom = {
			1.0f, 0.0f, 0.0f, 4.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	void RTAppTest3D::OnUpdate(float dtms)
	{
		m_Controller.UpdateEntity(dtms);
	}

	void RTAppTest3D::OnRender()
	{
		Render::ConsolerRenderer::Render3D(m_Scene, m_Camera);
	}

	void RTAppTest3D::OnQuit()
	{}

	CREATE_APP(RTAppTest3D)
}