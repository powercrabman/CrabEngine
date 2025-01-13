#pragma once
#include "Macro.h"

namespace crab
{
	enum class eRenderAPI
	{
		OpenGL = 0,
		DirectX11,
		DirectX12,
		Vulkan
	};

	enum class eDataFormat
	{
		None = 0,

		Int32,
		UInt32,

		Float,
		Float2,
		Float3,
		Float4,

		UNorm
	};

	enum class eBlendState
	{
		Opaque,
		AlphaBlend,
		Additive,
		NonPremultiplied,

		Count,
		Unknown
	};

	enum class eDepthStencilState
	{
		DepthNone,
		DepthDefault,
		DepthRead,
		DepthReverseZ,
		DepthReadReverseZ,

		Count,
		Unknown
	};

	enum class eRasterizerState
	{
		CullNone,
		CullClockwise,
		CullCounterClockwise,
		Wireframe,

		Count,
		Unknown
	};

	enum class eSamplerState
	{
		PointWrap,
		PointClamp,
		LinearWrap,
		LinearClamp,
		AnisotropicWrap,
		AnisotropicClamp,

		Count,
		Unknown
	};

	using eShaderFlags = unsigned int;
	enum eShaderFlags_
	{
		eShaderFlags_VertexShader = 1 << 0,
		eShaderFlags_PixelShader = 1 << 1
	};

	enum class eConstantBufferSlot
	{
		TransformSlot = 0,
		CameraSlot = 1,
		TextureSlot = 2
	};

	enum class eEditorState
	{
		Runtime,
		Edit,
		SimulatePlay,
		SimulateStop,
	};

	enum class eGuizmoType
	{
		Translation,
		Rotation,
		Scaling
	};

	//===================================================
	//			           Assets
	//===================================================

	enum class eAssetType
	{
		Mesh = 0,
		GameTexture,
		Flipbook,
		Sprite,
		MonoScript,
		Count
	};

	enum { eAssetTypeCount = static_cast<uint32_t>(eAssetType::Count) };

	constexpr const char* ToString(eAssetType in_type)
	{
		switch (in_type)
		{
		case eAssetType::Mesh:		  return "Mesh";
		case eAssetType::GameTexture: return "GameTexture";
		case eAssetType::Flipbook:	  return "Flipbook";
		case eAssetType::Sprite:	  return "Sprite";
		case eAssetType::MonoScript:  return "MonoScript";
		default: assert(false);		  return "";
		}
	}

	//===================================================
	//			         Component
	//===================================================

	enum class eProjection
	{
		Orthographic,
		Perspective
	};

}
