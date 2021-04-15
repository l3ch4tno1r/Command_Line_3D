#pragma once

#define NOMINMAX
#include <Windows.h>

#include <string>
#include <sstream>
#include <thread>
#include <functional>

#include "LCN_Math/Source/Matrix/MatrixBlock.h"
#include "LCN_Math/Source/Geometry/Transform.h"
#include "LCN_Math/Source/Geometry/Vector.h"

#include "Device.h"

#ifndef UNICODE
#error Please enable UNICODE for your compiler ! VS : \
Project Properties -> General -> Character Set -> Use Unicode.
#endif

#define TEST_CONSOLE 0

#if TEST_CONSOLE
#include <functional>
#include <mutex>
#include <condition_variable>

using namespace std::literals::chrono_literals;
#endif // TEST_CONSOLE

namespace LCN::Core
{
	enum COLOUR
	{
		FG_BLACK        = 0x0000,
		FG_DARK_BLUE    = 0x0001,
		FG_DARK_GREEN   = 0x0002,
		FG_DARK_CYAN    = 0x0003,
		FG_DARK_RED     = 0x0004,
		FG_DARK_MAGENTA = 0x0005,
		FG_DARK_YELLOW  = 0x0006,
		FG_GREY         = 0x0007,
		FG_DARK_GREY    = 0x0008,
		FG_BLUE         = 0x0009,
		FG_GREEN        = 0x000A,
		FG_CYAN         = 0x000B,
		FG_RED          = 0x000C,
		FG_MAGENTA      = 0x000D,
		FG_YELLOW       = 0x000E,
		FG_WHITE        = 0x000F,
		BG_BLACK        = 0x0000,
		BG_DARK_BLUE    = 0x0010,
		BG_DARK_GREEN   = 0x0020,
		BG_DARK_CYAN    = 0x0030,
		BG_DARK_RED     = 0x0040,
		BG_DARK_MAGENTA = 0x0050,
		BG_DARK_YELLOW  = 0x0060,
		BG_GREY         = 0x0070,
		BG_DARK_GREY    = 0x0080,
		BG_BLUE         = 0x0090,
		BG_GREEN        = 0x00A0,
		BG_CYAN         = 0x00B0,
		BG_RED          = 0x00C0,
		BG_MAGENTA      = 0x00D0,
		BG_YELLOW       = 0x00E0,
		BG_WHITE        = 0x00F0,
	};

	using MapFunction = std::function<CHAR_INFO(int x, int y)>;

	const auto defaultMapper = [](int, int)
	{
		CHAR_INFO result;

		result.Char.UnicodeChar = 0;
		result.Attributes = COLOUR::BG_WHITE;

		return result;
	};

	class Console : public Device
	{
	private:
		const float cm_ScreenDist = 0.1f;

	private:
		UINT32 m_Width;
		UINT32 m_Height;
		UINT32 m_Size;

		CHAR_INFO* m_ScreenBuffer;
		HANDLE     m_HConsole;
		SMALL_RECT m_RectWindow;

		float        m_Focal;
		Transform3Df m_R0ToCam;

		Console();

		~Console();

		Console(const Console&) = delete;
		Console& operator=(const Console&) = delete;

		void MainThread() override;

		using Pixel = VectorND<int, 2, HomogeneousVector>;
		//using Pixel = HVector2D<int>;

		struct Triangle2D
		{
			union
			{
				struct
				{
					Pixel p1;
					Pixel p2;
					Pixel p3;
				};
				Pixel pixels[3];
			};
		};

	public:
		static Console& Get() noexcept;

		void ConstructConsole(size_t width, size_t height, size_t fontw, size_t fonth);

		void Start() override;

		inline Transform3Df& R0ToCam()
		{
			return m_R0ToCam;
		}

		inline float& Focal()
		{
			return m_Focal;
		}

		inline UINT32 Width() const
		{
			return m_Width;
		}

		inline UINT32 Height() const
		{
			return m_Height;
		}

	#if TEST_CONSOLE

	#define WAIT if(!Wait()) return
	#define RENDER_AND_WAIT Render(); WAIT

	private:
		std::mutex              m_PauseMut;
		std::condition_variable m_PauseCondition;
		bool                    m_PauseNotified;
		bool                    m_Run;

		bool Wait();

	public:
		void Notify(bool run);

	#endif // TEST_CONSOLE

		/////////////////////////
		//-- Drawing methods --//
		/////////////////////////

		void Clear();

		void DrawPoint(int x, int y, short c = 0x2588, short col = 0x000F);

		void DrawPoint(int x, int y, const MapFunction& mapper);

		void DrawLine(int x1, int y1, int x2, int y2, short c = 0, short color = COLOUR::BG_WHITE);

		void FillScreen(const MapFunction& mapper = defaultMapper);

		void FillRectangle(int TLx, int TLy, int BRx, int BRy, const MapFunction& mapper = defaultMapper);

		void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const MapFunction& mapper = defaultMapper);

		void FillTriangleOLC(int x1, int y1, int x2, int y2, int x3, int y3, const MapFunction& mapper = defaultMapper);

		void Render();

	private:

		// TODO : Include this in a collision detection API ?
		static Vector3Df SegmentPlaneIntersection(const Vector3Df& v1, const Vector3Df& v2, // Segment info
												  const Vector3Df& n,  const Vector3Df& p); // Plane info

		// TODO : Improve that !!!
		static size_t ClipEdge(const Vector3Df& v1, const Vector3Df& v2, // Edge
							   const Vector3Df& n,  const Vector3Df& p,  // Plane parameters
									 Vector3Df& o1,       Vector3Df& o2);

		struct Triangle
		{
			Vector3Df vertices[3];
		};

		static size_t ClipTriangle(const Triangle&  in_t,                         // Triangle
								   const Vector3Df& n,    const Vector3Df& p,     // Plane parameters
										 Triangle&  o_t1,       Triangle&  o_t2); // Output triangles

		CHAR_INFO GetPixelValue(int x, int y) const;

	public:
		enum MessageSlots
		{
			_1 = 5,
			_2 = 4,
			_3 = 3,
			_4 = 2,
			_5 = 1,
		};

		class ConsoleMessage
		{
		public:
			~ConsoleMessage();

			friend Console;

			template<class DataType>
			ConsoleMessage& operator<<(const DataType& data);

		private:
			ConsoleMessage() = default;
			ConsoleMessage(MessageSlots slot);

		private:
			std::stringstream m_Sstr;
			MessageSlots      m_Slot;
		};

		ConsoleMessage Message(MessageSlots slot) const;

	private:
		void DisplayMessage(const std::string& msg, MessageSlots slot);

	private:
		void HeartBeat();
	};

	template<class DataType>
	inline Console::ConsoleMessage& Console::ConsoleMessage::operator<<(const DataType& data)
	{
		m_Sstr << data;

		return *this;
	}
}