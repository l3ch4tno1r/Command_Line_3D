#pragma once

#include <Utilities/Source/Design Patterns/Observer.h>

#include <functional>

namespace LCN
{
	//////////////
	//-- Slot --//
	//////////////

	template<class F>
	class Slot;

	template<class ...Args>
	class Slot<void(Args...)> : public Observer<Slot<void(Args...)>>
	{
	public:
		using FunctionType = std::function<void(Args...)>;

		Slot(const FunctionType& f) :
			m_Slot(f)
		{}

		Slot(FunctionType&& f) :
			m_Slot(std::move(f))
		{}

		void Update(Args&& ...args)
		{
			m_Slot(std::forward<Args>(args)...);
		}

		inline void operator()(Args&& ...args)
		{
			m_Slot(std::forward<Args>(args)...);
		}

	private:
		 FunctionType m_Slot;
	};

	////////////////
	//-- Signal --//
	////////////////

	template<class F>
	class Signal;

	template<class ...Args>
	class Signal<void(Args...)> : public Subject<Slot<void(Args...)>>
	{
	public:
		inline void operator()(Args&& ...args)
		{
			this->Notify(std::forward<Args>(args)...);
		}
	};

	template<class F>
	void Bind(Signal<F>& signal, Slot<F>& slot) { signal.AddObserver(slot); }

#define BIND_SLOT(Method) std::bind(&Method, this)
}