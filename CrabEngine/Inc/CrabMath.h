#pragma once

namespace crab::math
{
	constexpr float PI = 3.141592f;
	constexpr float PI_DIV2 = PI * 0.5f;
	constexpr float PI_DIV4 = PI * 0.25f;
	constexpr float SMALL_NUM = 1e-6;

	constexpr FORCE_INLINE NO_DISCARD float DegToRad(float in_degree)
	{
		return in_degree * PI / 180.f;
	}

	constexpr FORCE_INLINE NO_DISCARD float RadToDeg(float in_rad)
	{
		return in_rad * 180.f / PI;
	}
}