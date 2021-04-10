#pragma once

#include "Renderer.h"

namespace LCN::Core
{
	class Console;
}

namespace LCN::Render
{
	using ConsolerRenderer = Renderer<LCN::Core::Console>;
}