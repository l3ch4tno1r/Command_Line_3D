#include "RTAppTest.h"

#include <iostream>

namespace LCN
{
	RTAppTest::RTAppTest() :
		SLOT_INIT(SlotOnHeartbeat, RTAppTest::OnHeartbeat)
	{
		Connect(this->Heartbeat, this->SlotOnHeartbeat);
	}

	void RTAppTest::OnHeartbeat()
	{
		std::cout << "Heartbeat." << std::endl;
	}
}
