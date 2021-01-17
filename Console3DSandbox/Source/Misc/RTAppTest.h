#pragma once

#include <Console3D/Source/Core/RTApplication.h>

namespace LCN
{
	class RTAppTest : public RTApplication
	{
	public:
		RTAppTest();

	public:
		SLOT(RTAppTest, OnStartup);
		SLOT(RTAppTest, OnUpdate, float);
		SLOT(RTAppTest, OnRender);
		SLOT(RTAppTest, OnQuit);
	};
}

using LCNApp = LCN::RTAppTest;