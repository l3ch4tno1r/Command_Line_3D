#include <Console3D/Source/Core/EventApplication.h>
#include <Console3D/Source/Core/Console.h>

namespace LCN
{
	class CollisionTestApp : public Core::EventApplication
	{
	public:
		CollisionTestApp() :
			SLOT_INIT(SlotOnStartup, CollisionTestApp::OnStartup)
		{
			Connect(this->SignalStartup, this->SlotOnStartup);
		}

	private: // Slots
		SLOT(CollisionTestApp, OnStartup);
	};

	void CollisionTestApp::OnStartup()
	{
		auto& console = Core::Console::Get();

		console.ConstructConsole(300, 200, 4, 4);
	}

	Core::Application::AppPointer Core::Application::CreateApplication()
	{
		return std::make_unique<CollisionTestApp>();
	}
}

#include <Console3D/Source/Core/EntryPoint.h>