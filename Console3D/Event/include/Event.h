#pragma once

namespace LCN
{
	class Event
	{
	public:
		bool Handled = false;

		virtual ~Event() = default;
	};
}
