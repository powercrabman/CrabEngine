#pragma once

namespace crab
{
	struct alignas(16) CBTransform
	{
		inline static eConstantBufferSlot s_slot = eConstantBufferSlot::TransformSlot;

		Mat worldMat = Mat::Identity;
	};

	struct alignas(16) CBCamera
	{
		inline static eConstantBufferSlot s_slot = eConstantBufferSlot::CameraSlot;

		Mat		viewProjMat = Mat::Identity;
		Vec3	cameraPosition = Vec3::Zero;
	};

	struct alignas(16) CBTexture
	{
		inline static eConstantBufferSlot s_slot = eConstantBufferSlot::TextureSlot;

		Vec2 uv1;
		Vec2 uv2;
	};
}