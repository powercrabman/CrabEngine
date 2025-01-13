#pragma once

namespace crab::Color
{
	constexpr Vec4 RED = { 1.f, 0.f, 0.f, 1.f };
	constexpr Vec4 GREEN = { 0.f, 1.f, 0.f, 1.f };
	constexpr Vec4 BLUE = { 0.f, 0.f, 1.f, 1.f };
	constexpr Vec4 WHITE = { 1.f, 1.f, 1.f, 1.f };
	constexpr Vec4 BLACK = { 0.f, 0.f, 0.f, 1.f };
	constexpr Vec4 ALPHA = { 0.f, 0.f, 0.f, 0.f };
}

namespace crab
{
	constexpr uint8 ExtractR(uint32 rgb)
	{
		return static_cast<uint8>(rgb & 0xFF);
	}

	constexpr uint8 ExtractG(uint32 rgb)
	{
		return static_cast<uint8>((rgb >> 8) & 0xFF);
	}

	constexpr uint8 ExtractB(uint32 rgb)
	{
		return static_cast<uint8>((rgb >> 16) & 0xFF);
	}

	constexpr float BYTE_COLOR_INV = 1 / 255.f;
	constexpr float ExtractRNorm(uint32 rgb)
	{
		return static_cast<uint8>(rgb & 0xFF) * BYTE_COLOR_INV;
	}

	constexpr float ExtractGNorm(uint32 rgb)
	{
		return static_cast<uint8>((rgb >> 8) & 0xFF) * BYTE_COLOR_INV;
	}

	constexpr float ExtractBNorm(uint32 rgb)
	{
		return static_cast<uint8>((rgb >> 16) & 0xFF) * BYTE_COLOR_INV;
	}
}