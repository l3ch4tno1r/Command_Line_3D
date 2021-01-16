#pragma once

#include "Event.h"

namespace LCN
{
	////////////////////
	//-- MouseEvent --//
	////////////////////

	class MouseEvent : public Event
	{
	public:
		MouseEvent(int x, int y) :
			m_X(x),
			m_Y(y)
		{}

		inline int X() const { return m_X; }
		inline int Y() const { return m_Y; }

	private:
		int m_X, m_Y;
	};

	/////////////////////////
	//-- MouseMovedEvent --//
	/////////////////////////

	class MouseMovedEvent : public MouseEvent
	{
	public:
		MouseMovedEvent(int x, int y) :
			MouseEvent(x, y)
		{}
	};

	//////////////////////////
	//-- MouseButtonEvent --//
	//////////////////////////

	class MouseButtonEvent : public MouseEvent
	{
	public:
		MouseButtonEvent(int x, int y, unsigned short code) :
			MouseEvent(x, y),
			m_ButtonCode(code)
		{}

		unsigned short ButtonCode() const { return m_ButtonCode; }

	private:
		unsigned short m_ButtonCode;
	};

	/////////////////////////////////
	//-- MouseButtonPressedEvent --//
	/////////////////////////////////

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int x, int y, unsigned short code) :
			MouseButtonEvent(x, y, code)
		{}
	};

	//////////////////////////////////
	//-- MouseButtonReleasedEvent --//
	//////////////////////////////////

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int x, int y, unsigned short code) :
			MouseButtonEvent(x, y, code)
		{}
	};

	//////////////////////////
	//-- MouseScrollEvent --//
	//////////////////////////

	class MouseScrollEvent : public MouseEvent
	{
	public:
		MouseScrollEvent(int x, int y, short scrolldir) :
			MouseEvent(x, y),
			m_ScrollDir(scrolldir)
		{}

		short ScrollDirection() const { return m_ScrollDir; }

	private:
		short m_ScrollDir;
	};
}