#pragma once

#include <Utilities/Source/Design Patterns/Observer.h>

#include <functional>

namespace LCN
{
	//////////////////
	//-- SlotBase --//
	//////////////////

	template<class F>
	class SlotBase;

	template<class ...Args>
	class SlotBase<void(Args...)> : public Observer<SlotBase<void(Args...)>>
	{
	public:
		virtual inline void operator()(Args&&...) = 0;

		void Update(Args&&... args)
		{
			this->operator()(std::forward<Args>(args)...);
		}

	protected:
		virtual ~SlotBase() = default;
	};

	//////////////
	//-- Slot --//
	//////////////

	template<class O, class F>
	class Slot;

	template<class Owner, class ...Args>
	class Slot<Owner, void(Args...)> : public SlotBase<void(Args...)>
	{
	public:
		inline void operator()(Args&& ...args)
		{
			m_Slot(std::forward<Args>(args)...);
		}

	private:
		using FunctionType = std::function<void(Args...)>;

		typedef void(Owner::*MethodPtrType)(Args...);

		Slot(Owner& owner, MethodPtrType method) :
			m_Slot(std::bind(method, std::ref(owner)))
		{}

		friend Owner;
		FunctionType m_Slot;
	};

	////////////////
	//-- Signal --//
	////////////////

	template<class F>
	class Signal;

	template<class ...Args>
	class Signal<void(Args...)> : public Subject<SlotBase<void(Args...)>>
	{
	public:
		inline void operator()(Args&& ...args)
		{
			this->Notify(std::forward<Args>(args)...);
		}

		inline void Emmit(Args&& ...args)
		{
			this->Notify(std::forward<Args>(args)...);
		}
	};

	///////////////
	//-- Utils --//
	///////////////

	template<class F>
	void Bind(Signal<F>& signal, SlotBase<F>& slot) { signal.AddObserver(slot); }

#define INIT_SLOT(Method) *this, &Method
}