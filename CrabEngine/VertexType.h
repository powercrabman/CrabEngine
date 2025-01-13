#pragma once

namespace crab
{
	struct VertexPosColor
	{
		Vec3 position;
		Vec4 color;
	};

	struct VertexPosTex
	{
		Vec3 position;
		Vec2 tex;
	};

	struct VertexPosTexColor
	{
		Vec3 position;
		Vec2 tex;
		Vec4 color;
	};
}