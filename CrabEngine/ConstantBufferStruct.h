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

	struct alignas(16) CBEffect
	{
		inline static eConstantBufferSlot	s_slot = eConstantBufferSlot::TextureSlot;
		inline static eShaderFlags			s_bindFlags = eShaderFlags_PixelShader;
	
		bool useTexture = false;
	};
}