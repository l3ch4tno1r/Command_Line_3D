#include <Console3D/Source/Core/EventApplication.h>
#include <Console3D/Source/Core/Console.h>

#include <Console3D/Source/Rendering/Renderer2D.h>

#include <Console3D/Source/Scene/Scene.h>
#include <Console3D/Source/Scene/Entity.h>
#include <Console3D/Source/Scene/StdComponent.h>

namespace LCN
{

	class CollisionTestApp : public Core::EventApplication
	{
	public:
		CollisionTestApp()
		{
			Connect(this->SignalStartup,    this->SlotOnStartup);
			Connect(this->SignalMouseMoved, this->SlotOnMouseMove);
		}

	private: // Slots
		SLOT(CollisionTestApp, OnStartup);
		SLOT(CollisionTestApp, OnUpdate);
		SLOT(CollisionTestApp, OnMouseMove, MouseMovedEvent&);

	private:
		Entity m_CameraEntt;
		Entity m_MeshEntt;
		Entity m_R0Frame;
		Scene  m_Scene;

		Controller::Camera2DController m_Controller;
	};

	void CollisionTestApp::OnStartup()
	{
		auto& console = Core::Console::Get();

		console.ConstructConsole(300, 200, 4, 4);
		//console.ConstructConsole(150, 100, 8, 8);

		// Create camera entity
		m_CameraEntt = m_Scene.CreateEntity();
		m_CameraEntt.Add<Component::Camera2DCmp>(console.Width(), console.Height());

		// Bind camera controller
		m_Controller.Bind(m_CameraEntt);

		Connect(m_Controller.SignlaUpdate, this->SlotOnUpdate);

		// Create mesh entity
		m_MeshEntt = m_Scene.CreateEntity();
		m_MeshEntt.Add<Component::Sprite2DCmp>(100, 100);

		// Create frame representation entity
		m_R0Frame = m_Scene.CreateEntity();
		m_R0Frame.Add<Component::FrameCmp>(10.0f);

		Render::Renderer2D::Clear();
		Render::Renderer2D::RenderWireFrame(m_Scene, m_CameraEntt);
		Render::Renderer2D::Submit();
	}

	void CollisionTestApp::OnMouseMove(MouseMovedEvent& mouseEvent)
	{
		static auto& console = Core::Console::Get();

		Render::Renderer2D::Clear();

		Render::Renderer2D::RenderWireFrame(m_Scene, m_CameraEntt);

		// Draw crosshair
		console.DrawLine(
			mouseEvent.X() - 5, mouseEvent.Y(),
			mouseEvent.X() + 5, mouseEvent.Y(),
			0, Core::COLOUR::BG_RED);

		console.DrawLine(
			mouseEvent.X(), mouseEvent.Y() - 5,
			mouseEvent.X(), mouseEvent.Y() + 5,
			0, Core::COLOUR::BG_RED);

		Render::Renderer2D::Submit();
	}

	void CollisionTestApp::OnUpdate()
	{
		Render::Renderer2D::Clear();
		Render::Renderer2D::RenderWireFrame(m_Scene, m_CameraEntt);
		Render::Renderer2D::Submit();
	}

	CREATE_APP(CollisionTestApp)
}

#include <Console3D/Source/Core/EntryPoint.h>