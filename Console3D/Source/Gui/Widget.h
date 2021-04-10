#pragma once

#include <vector>
#include <memory>

namespace LCN
{
	class CWidget
	{
	public:
		CWidget();

	private:
		CWidget* m_Parent = nullptr;
		std::vector<CWidget*> m_Children;
	};
}