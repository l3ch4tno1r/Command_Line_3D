#pragma once

#include <mutex>
#include <condition_variable>

#include <Utilities/Source/DesignPatterns/SignalSlot.h>

int main(int argc, char** argv);

namespace LCN
{
	template<class _Derived>
	class Application
	{
	public:
		static Application& Get() noexcept
		{
			static _Derived instance;
			return instance;
		}

		void Quit()
		{
			std::lock_guard<std::mutex> lock(m_RunMut);

			m_Run = false;

			m_RunCond.notify_one();
		}
		Slot<Application, void()> SlotQuit;

	protected:
		inline _Derived& Derived() { return static_cast<_Derived&>(*this); }
		inline const _Derived& Derived() const { return static_cast<_Derived&>(*this); }

		Application() :
			SlotQuit(INIT_SLOT(Application::Quit))
		{
			if (m_App)
				throw std::exception("Application is already running.");

			m_App = this;
		}
	
		virtual ~Application() = default;

		void WaitQuit()
		{
			std::unique_lock<std::mutex> lock(m_RunMut);

			while (m_Run)
				m_RunCond.wait(lock);
		}

	private:
		void Run() { this->Derived().Run(); }

		friend int ::main(int argc, char** argv);

	private:
		inline static Application* m_App = nullptr;

		bool                    m_Run = true;
		std::mutex              m_RunMut;
		std::condition_variable m_RunCond;
	};
}