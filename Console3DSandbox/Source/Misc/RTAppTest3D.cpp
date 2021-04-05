#include "RTAppTest3D.h"

#include "Console3D/Source/Rendering/ConsoleRenderer.h"
#include "Console3D/Source/Scene/StdComponent.h"

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

		// Setup ifinite chessboard floor entity
		m_Floor = m_Scene.Create3DEntity();

		m_Floor.Add<Component::InfiniteChessboardCmp>(1, 1);
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