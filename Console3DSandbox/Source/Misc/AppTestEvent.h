#pragma once

#include <Console3D/Source/Core/Application.h>

namespace LCN
{
	class AppTestEvent : public Application<AppTestEvent>
	{
	public:
		void Run();
	};
}

using LCNApp = LCN::AppTestEvent;