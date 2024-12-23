#pragma once

namespace crab
{
	enum class eRenderAPI
	{
		OpenGL = 0,
		DirectX11,
		DirectX12,
		Vulkan
	};

	enum class eBlendState
	{
		Opaque,
		AlphaBlend,
		Additive,
		NonPremultiplied,

		Count
	};

	enum class eDepthStencilState
	{
		DepthNone,
		DepthDefault,
		DepthRead,
		DepthReverseZ,
		DepthReadReverseZ,

		Count
	};

	enum class eRasterizerState
	{
		CullNone,
		CullClockwise,
		CullCounterClockwise,
		Wireframe,

		Count
	};

	enum class eSamplerState
	{
		PointWrap,
		PointClamp,
		LinearWrap,
		LinearClamp,
		AnisotropicWrap,
		AnisotropicClamp,

		Count
	};

	using eShaderFlags = unsigned int;
	enum eShaderFlags_
	{
		eShaderFlags_VertexShader = 1 << 0,
		eShaderFlags_PixelShader  = 1 << 1
	};

	enum class eConstantBufferSlot
	{
		TransformSlot = 0,
		CameraSlot    = 1
	};

	enum class eRunType
	{
		Runtime,
		Edit,
		SimulatePlay,
		SimulateStop,
	};
}