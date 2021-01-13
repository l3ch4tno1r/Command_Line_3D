#pragma once

#define NOMINMAX
#include <Windows.h>

#include <thread>
#include <atomic>

#include <Utilities/Source/DesignPatterns/SignalSlot.h>
#include <Console3D/Source/Event/KeyBoardEvent.h>

namespace LCN
{

	enum Key
	{
		ENTER  = VK_RETURN,
		ESC    = VK_ESCAPE,
		LSHIFT = VK_LSHIFT,
		LCTRL  = VK_LCONTROL,

		A      = 0x41,
		B      = 0x42,
		C      = 0x43,
		D      = 0x44,
		E      = 0x45,
		F      = 0x46,
		G      = 0x47,
		H      = 0x48,
		I      = 0x49,
		J      = 0x4A,
		K      = 0x4B,
		L      = 0x4C,
		M      = 0x4D,
		N      = 0x4E,
		O      = 0x4F,
		P      = 0x50,
		Q      = 0x51,
		R      = 0x52,
		S      = 0x53,
		T      = 0x54,
		U      = 0x55,
		V      = 0x56,
		W      = 0x57,
		X      = 0x58,
		Y      = 0x59,
		Z      = 0x5A
	};

	enum MouseButton
	{
		LEFT   = 0,
		RIGHT  = 1,
		MIDDLE = 2
	};

	struct KeyState
	{
		bool KeyPressed  : 1;
		bool KeyReleased : 1;
		bool KeyHeld     : 1;
		bool _NA         : 3;
		bool KeyOldState : 1;
		bool KeyNewState : 1;
	};
		
	using MouseAction     = std::function<void(const KeyState&, int, int)>;
	using MouseMoveAction = std::function<void(int, int)>;

	class EventHandler
	{
	public:
		static EventHandler& Get() noexcept;

		void Start();
		void Stop();

	public: // Signals
		Signal<void(KeyPressedEvent&)>  SignalKeyPressed;
		Signal<void(KeyReleasedEvent&)> SignalKeyReleased;

	private:
		EventHandler();
		~EventHandler();

		void MainThread();

	private:
		HANDLE m_HStdIn;

		std::thread      m_MainThread;
		std::atomic_bool m_Run = false;

		KeyState m_Keys[256], m_Mouse[5];

		MouseAction     m_MouseActions[5];
		MouseMoveAction m_MouseMoveAction;
	};
}