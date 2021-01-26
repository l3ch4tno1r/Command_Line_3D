#pragma once

namespace LCN
{
	class Entity;

	namespace Behaviour
	{
		class RenderBehaviour
		{
		public:
			virtual void Render(size_t modelid) = 0;
		};
	}
}