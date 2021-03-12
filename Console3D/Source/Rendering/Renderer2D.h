#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Console3D/Source/Scene/Scene.h"
#include "Console3D/Source/Scene/Entity.h"
#include "Console3D/Source/Core/Console.h"

namespace LCN::Render
{
	struct ViewPort
	{
		int TLx, TLy;
		int BRx, BRy;
	};

	class Renderer2D
	{
	public:
		static void Init();



		static void Render();

		static void RenderSequential(
			Scene& scene,
			Entity cameraEntity,
			const ViewPort& viewPort = {0, 0, (int)Core::Console::Get().Width(), (int)Core::Console::Get().Height() });

		static void RenderParallel(
			Scene& scene,
			Entity cameraEntity,
			const ViewPort& viewPort = { 0, 0, (int)Core::Console::Get().Width(), (int)Core::Console::Get().Height() });

	private:
		std::thread             m_ThreadPool[4];
		std::atomic_bool        m_Run = false;
		std::mutex              m_Mutex;
		std::condition_variable m_Condition;

		static Renderer2D& Get() noexcept;

	private:
		Renderer2D() = default;

		~Renderer2D();

		void RenderThread();
	};
}