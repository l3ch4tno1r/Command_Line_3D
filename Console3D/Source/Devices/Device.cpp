#include "Device.h"

bool SyncDevice::Continue()
{
	std::unique_lock<std::mutex> lock(m_NotifMut);

	while (!m_State.Notified)
		m_NotifCond.wait(lock);

	m_State.Notified = false;

	return m_State.Run;
}

SyncDevice::~SyncDevice()
{
	Update(false);

	if (m_MainThread.joinable())
		m_MainThread.join();
}

void SyncDevice::Start()
{
	if (m_State.Run)
		return;

	m_State.Run = true;

	m_MainThread = std::thread(&SyncDevice::MainThread, this);
}

void SyncDevice::Update(bool run)
{
	std::lock_guard<std::mutex> lock(m_NotifMut);

	m_State.Run = run;
	m_State.Notified = true;

	m_NotifCond.notify_one();
}