#pragma once

namespace crab::math
{
	constexpr float PI        = 3.141592f;
	constexpr float PI2       = PI*2.f;
	constexpr float PI_DIV2   = PI * 0.5f;
	constexpr float PI_DIV4   = PI * 0.25f;
	constexpr float SMALL_NUM = 1e-4;

	constexpr FORCE_INLINE NODISCARD float DegToRad(float in_degree)
	{
		return in_degree * PI / 180.f;
	}

	constexpr FORCE_INLINE NODISCARD float RadToDeg(float in_rad)
	{
		return in_rad * 180.f / PI;
	}

	constexpr FORCE_INLINE NODISCARD Vec3 DegVecToRadVec(const Vec3& in_srcVec)
	{
		return Vec3{
			DegToRad(in_srcVec.x),
			DegToRad(in_srcVec.y),
			DegToRad(in_srcVec.z)
		};
	}

	constexpr FORCE_INLINE NODISCARD Vec3 RadVecToDegVec(const Vec3& in_srcVec)
	{
		return Vec3{
			RadToDeg(in_srcVec.x),
			RadToDeg(in_srcVec.y),
			RadToDeg(in_srcVec.z)
		};
	}

	FORCE_INLINE NODISCARD auto CalcSinCos(float in_rad)
	{
		float sin, cos;
		DirectX::XMScalarSinCos(&sin, &cos, in_rad);
		return std::make_pair(sin, cos);
	}

	FORCE_INLINE NODISCARD float SinCosToRad(float in_sin, float in_cos)
	{
		return std::atan2(in_sin, in_cos);
	}

	NODISCARD int32 RandInt32(int32 low, int32 high);
	NODISCARD float RandFloat(float low, float high);

	template <typename Type>
	FORCE_INLINE NODISCARD bool IsBetweenWith(Type& in_target, const Type& in_low, const Type& in_high)
	{
		return in_low <= in_target && in_target <= in_high;
	}
}
