#include "Device.h"

bool Device::Continue()
{
	std::unique_lock<std::mutex> lock(m_NotifMut);

	while (!m_State.Notified)
		m_NotifCond.wait(lock);

	m_State.Notified = false;

	return m_State.Run;
}

Device::Device()
{
	m_State.Started  = false;
	m_State.Run      = false;
	m_State.Notified = false;
}

Device::~Device()
{
	Update(false);

	if (m_MainThread.joinable())
		m_MainThread.join();
}

void Device::Start()
{
	if (m_State.Started)
		return;

	m_State.Started = true;
	m_State.Run     = true;

	m_MainThread = std::thread(&Device::MainThread, this);
}

void Device::Update(bool run)
{
	std::lock_guard<std::mutex> lock(m_NotifMut);

	m_State.Run = run;
	m_State.Notified = true;

	m_NotifCond.notify_one();
}