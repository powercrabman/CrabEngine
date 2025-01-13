#pragma once

namespace crab
{
	struct TimeStamp
	{
		float	rawDeltaTime   = 0.f;	// 루프 당 소요 시간 (단위: 초)
		float	rawTotalTime   = 0.f;	// 프로그램 시작 후 소요 시간 (단위: 초)
		float	deltaTime      = 0.f;	// 루프 당 소요 시간, timeScale과 isPaused 등의 영향을 받음 (단위: 초)
		float	totalTime      = 0.f;	// 프로그램 시작 후 소요 시간, timeScale과 isPaused 등의 영향을 받음 (단위: 초)
		float	timeScale      = 1.f;	// 시간 배속. timeScale 이 클수록 시간이 더 빠르게 지남 기본값 1.f
		float	framePerSec    = 0.f;	// 초당 갱신 횟수
		bool	isPaused       = false;	// 시간 정지.
	};

	class Timer
	{
	public:
		Timer();
		~Timer() = default;

		void CalcTimeStamp(TimeStamp& in_out_timeStamp)
		{
			uint64 currentTime = SDL_GetPerformanceCounter();

			// 델타 타임 계산
			uint64 delta = currentTime - m_lastTime;
			m_lastTime = currentTime;

			// raw delta 계산
			in_out_timeStamp.rawDeltaTime = static_cast<float>(delta) / m_freq;

			// delta time 계산
			if (in_out_timeStamp.isPaused)
			{
				in_out_timeStamp.deltaTime = 0.f;
			}
			else
			{
				in_out_timeStamp.deltaTime = in_out_timeStamp.rawDeltaTime * in_out_timeStamp.timeScale;
			}

			// total time 계산
			in_out_timeStamp.totalTime += in_out_timeStamp.deltaTime;
			in_out_timeStamp.rawTotalTime += in_out_timeStamp.rawDeltaTime;
		}

	private:
		uint64	m_lastTime;
		float	m_freq;
	};
}