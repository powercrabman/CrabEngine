#include "CrabEnginePch.h"
#include "Timer.h"

namespace crab
{
	Timer::Timer()
	{
		m_lastTime = SDL_GetPerformanceCounter();
		m_freq = static_cast<float>(SDL_GetPerformanceFrequency());
	}
}
