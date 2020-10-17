#pragma once

#include "Device.h"

class GraphicEngine : public Device
{
private:
	void MainThread() override;
};