#pragma once

#include "Device.h"

namespace LCN::Core
{
	class GraphicEngine : public Device
	{
	private:
		void MainThread() override;
	};
}