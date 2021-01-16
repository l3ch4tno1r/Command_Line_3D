#pragma once

#include <Console3D/Source/Core/RTApplication.h>

namespace LCN
{
	class RTAppTest : public RTApplication
	{
	public:
		RTAppTest();

	public:
		SLOT(RTAppTest, OnHeartbeat);
	};
}

using LCNApp = LCN::RTAppTest;