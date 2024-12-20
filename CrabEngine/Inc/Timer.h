#pragma once

namespace crab
{
	class Timer
	{
	public:
		Timer();
		~Timer() = default;

		float CalcDeltaTime()
		{
			Uint64 currentTime = SDL_GetPerformanceCounter();
			float deltaTime = static_cast<float>(currentTime - m_lastTime) / m_freq;
			m_lastTime = currentTime;
			return deltaTime;
		}

	private:
		Uint64	m_lastTime;
		float	m_freq;
	};
}