#pragma once
#include "Enums.h"
#include "IRenderTarget.h"

namespace crab
{

	inline uint32 GetDataFormatSize(eDataFormat in_format)
	{
		switch (in_format)
		{
		case crab::eDataFormat::Int32:  return 4;
		case crab::eDataFormat::UInt32: return 4;

		case crab::eDataFormat::Float:  return 4;
		case crab::eDataFormat::Float2: return 4 * 2;
		case crab::eDataFormat::Float3: return 4 * 3;
		case crab::eDataFormat::Float4: return 4 * 4;

		case crab::eDataFormat::UNorm: return 4;
		default: assert(false); return 0;
		}
	}

	struct LayoutElement
	{
		std::string		name;
		uint32			index;
		eDataFormat		format;
	};

	//===================================================
	//				      DirectX11
	//===================================================

	inline DXGI_FORMAT CvtDataFormatToDXGIFormat(eDataFormat in_format)
	{
		switch (in_format)
		{
		case eDataFormat::Int32: return DXGI_FORMAT_R32_SINT;
		case eDataFormat::UInt32: return DXGI_FORMAT_R32_UINT;

		case eDataFormat::Float: return DXGI_FORMAT_R32_FLOAT;
		case eDataFormat::Float2: return DXGI_FORMAT_R32G32_FLOAT;
		case eDataFormat::Float3: return DXGI_FORMAT_R32G32B32_FLOAT; 
		case eDataFormat::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case eDataFormat::UNorm: return DXGI_FORMAT_R8G8B8A8_UNORM;
		default: assert(false); return DXGI_FORMAT_UNKNOWN;
		}
	}


}
