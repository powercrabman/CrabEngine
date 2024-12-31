#pragma once

namespace crab
{
	struct alignas(16) CBTransform
	{
		inline static eConstantBufferSlot	s_slot = eConstantBufferSlot::TransformSlot;
		inline static eShaderFlags			s_bindFlags = eShaderFlags_VertexShader;

		Mat worldMat = Mat::Identity;
	};

	struct alignas(16) CBCamera
	{
		inline static eConstantBufferSlot	s_slot = eConstantBufferSlot::CameraSlot;
		inline static eShaderFlags			s_bindFlags = eShaderFlags_VertexShader;

		Mat		viewProjMat = Mat::Identity;
		Vec3	cameraPosition = Vec3::Zero;
	};

	struct alignas(16) CBTexture
	{
		inline static eConstantBufferSlot	s_slot = eConstantBufferSlot::TextureSlot;
		inline static eShaderFlags			s_bindFlags = eShaderFlags_VertexShader | eShaderFlags_PixelShader;

		Vec2	uv0;
		Vec2	uv1;
		uint32	useTexture;
	};
}