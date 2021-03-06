#pragma once

#define NOMINMAX
#include <Windows.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <tuple>
#include <type_traits>

#include <Utilities/Source/DesignPatterns/SignalSlot.h>
#include <Utilities/Source/Utils.h>

#include <Console3D/Source/Event/KeyBoardEvent.h>
#include <Console3D/Source/Event/MouseEvent.h>

namespace LCN::Core
{
	enum class Key
	{
		Enter = VK_RETURN,
		Esc   = VK_ESCAPE,
		Shift = VK_SHIFT,
		Ctrl  = VK_CONTROL,

		ArrowLeft  = VK_LEFT,
		ArrowUp    = VK_UP,
		ArrowRight = VK_RIGHT,
		ArrowDown  = VK_DOWN,

		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,

		SCREENSHOT = VK_SNAPSHOT
	};

	inline Key FromIntegral(char c)
	{
		c = c <= 'z' && 'a' <= c ? c - 'a' + 'A' : c;

		return static_cast<Key>(c);
	}

	inline std::istream& operator>>(std::istream& stream, Key& out)
	{
		char c;

		stream >> c;

		out = FromIntegral(c);

		return stream;
	}

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

	class ConsoleInput
	{
	public:
		static ConsoleInput& Get() noexcept;

		void Start();
		void Stop();

	public: // Signals
		template<class F>
		using SignalConsoleInput = Signal<ConsoleInput, F>;

		SignalConsoleInput<void(KeyPressedEvent&)>  SignalKeyPressed;
		SignalConsoleInput<void(KeyReleasedEvent&)> SignalKeyReleased;

		SignalConsoleInput<void(MouseMovedEvent&)>          SignalMouseMove;
		SignalConsoleInput<void(MouseScrollEvent&)>         SignalMouseScroll;
		SignalConsoleInput<void(MouseButtonPressedEvent&)>  SignalMouseButtonPressed;
		SignalConsoleInput<void(MouseButtonReleasedEvent&)> SignalMouseButtonReleased;

		static bool IsKeyPressed(Key key);
		static bool IsMouseBtnPressed(MouseButton mousebtn);

		std::tuple<int, int> GetWindowCenter() const;

		void SetCursorPosition(int x, int y);

		void Continue();

	private:
		void Wait();

		ConsoleInput();
		~ConsoleInput();

		void MainThread();

	private:
		HANDLE m_HStdIn;
		HWND   m_HWnd;

		std::thread             m_MainThread;
		std::atomic_bool        m_Run = false;
		std::mutex              m_ContinueMut;
		std::condition_variable m_ContinueCond;

		bool m_Notified = false;

		KeyState m_KeysState[256], m_Mouse[5];

		const long dwButtonStateHighWordMask = 0b11111111111110000000000000000000;
	};
}