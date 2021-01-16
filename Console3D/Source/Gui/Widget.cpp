#include "Widget.h"

namespace LCN
{
	Widget::Widget(Widget& parent) :
		m_Parent(&parent)
	{
		parent.m_Children.push_back(this);
	}

	void Widget::Render()
	{

	}
}
