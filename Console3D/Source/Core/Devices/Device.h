#pragma once

#include "Utilities/Source/Design Patterns/Observer.h"

class Device : public Observer<Device>
{
private:
	std::thread m_MainThread;

	struct
	{
		bool Started  : 1;
		bool Run      : 1;
		bool Notified : 1;
	} m_State;

	std::mutex              m_NotifMut;
	std::condition_variable m_NotifCond;

	virtual void MainThread() = 0;

protected:
	bool Continue();

public:
	Device();

	virtual ~Device();

	void Start();

	void Update(bool run);
};
