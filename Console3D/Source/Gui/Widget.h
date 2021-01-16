#pragma once

#include <vector>

namespace LCN
{
	class Widget
	{
	public:
		Widget() = default;
		Widget(Widget& parent);

		virtual void Render();

	private:
		Widget* m_Parent = nullptr;
		std::vector<Widget*> m_Children;
	};
}