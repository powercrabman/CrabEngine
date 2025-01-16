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
		None,			// 기존 설정을 유지
	};

	enum class eDepthStencilState
	{
		NoDepthTest,
		DepthTest,
		DepthOnlyRead,
		DepthReverseZ,
		DepthOnlyReadReverseZ,

		Count,
		None,			// 기존 설정을 유지
	};

	enum class eRasterizerState
	{
		NoCull,
		CullClockwise,
		CullCounterClockwise,
		Wireframe,

		Count,
		None,			// 기존 설정을 유지
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
		None,			// 기존 설정을 유지
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

	enum class eVisualLogLevel
	{
		Info, Warn, Error, Count
	};

	inline const char* ToString(eVisualLogLevel e)
	{
		switch (e)
		{
		case eVisualLogLevel::Info: return "Info";
		case eVisualLogLevel::Warn: return "Warn";
		case eVisualLogLevel::Error: return "Error";
		default: assert(false); return "unknown";
		}
	}

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
		Count,
		Unknown
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
