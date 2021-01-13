#pragma once

#include "Event.h"

namespace LCN
{
	//////////////////
	//-- KeyEvent --//
	//////////////////

	class KeyEvent : public Event
	{
	public:
		KeyEvent(unsigned short keycode) :
			m_KeyCode(keycode)
		{}

		virtual ~KeyEvent() = default;

		unsigned short KeyCode() const { return m_KeyCode; }

	private:
		unsigned short m_KeyCode;
	};

	////////////////////////
	//-- KeyPressedEvent--//
	////////////////////////

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(unsigned short keycode) :
			KeyEvent(keycode)
		{}
	};

	//////////////////////////
	//-- KeyReleasedEvent --//
	//////////////////////////

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(unsigned short keycode) :
			KeyEvent(keycode)
		{}
	};
}