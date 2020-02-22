#include "PaceMaker.h"

PaceMaker::PaceMaker() :
	m_Run(true),
	m_RunThread(&PaceMaker::Run, this)
{}

PaceMaker::~PaceMaker()
{
	m_Run = false;
	m_RunThread.join();
}

void PaceMaker::Run()
{
	while (m_Run)
	{

	}
}
