#include "RTAppTest3D.h"

#include "Console3D/Source/Rendering/ConsoleRenderer.h"

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
	}

	void RTAppTest3D::OnUpdate(float dtms)
	{}

	void RTAppTest3D::OnRender()
	{}

	void RTAppTest3D::OnQuit()
	{}

	CREATE_APP(RTAppTest3D)
}