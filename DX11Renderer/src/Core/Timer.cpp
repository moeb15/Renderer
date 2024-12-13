#include "Timer.h"

Yassin::Timer::Timer()
{
	m_LastTick = std::chrono::steady_clock::now();
}

float Yassin::Timer::Tick()
{
	const auto old = m_LastTick;
	m_LastTick = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameTime = m_LastTick - old;
	m_ElapsedMS += frameTime.count();
	return frameTime.count();
}

float Yassin::Timer::Peek() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_LastTick).count();
}
