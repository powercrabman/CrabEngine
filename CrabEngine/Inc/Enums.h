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
		TransformSlot  = 0,
		CameraSlot     = 1,
		TextureSlot    = 2
	};

	enum class eRunType
	{
		Runtime,
		Edit,
		SimulatePlay,
		SimulateStop,
	};

	/////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////

	enum class eAssetType
	{
		Mesh = 0,
		Flipbook,
		Sprite,
		Count
	};

	enum { eAssetTypeCount = static_cast<uint32_t>(eAssetType::Count) };

	constexpr const char* ToString(eAssetType in_type)
	{
		switch (in_type)
		{
		case eAssetType::Mesh:		return "Mesh";
		case eAssetType::Flipbook:	return "Flipbook";
		case eAssetType::Sprite:	return "Sprite";
		default: assert(false);		return "";
		}
	}

	/////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////

	enum class eProjection
	{
		Orthographic,
		Perspective
	};
}