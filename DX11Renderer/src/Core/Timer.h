#pragma once
#include <chrono>

namespace Yassin
{
	class Timer
	{
	public:
		Timer();

		/*
		* Elapsed time per frame
		*/
		float Tick();
		/*
		* Elapsed time since last Tick() invocation
		*/
		float Peek() const;

		/*
		* Elapsed time in miliseconds
		*/
		const float& GetElapsedTimeMS() const { return m_ElapsedMS; }

		/*
		* Elapsed time in seconds
		*/
		const float& GetElapsedTime() const { return m_ElapsedMS / 1000.f; }

	private:
		std::chrono::steady_clock::time_point m_LastTick;
		float m_ElapsedMS;
	};
}
