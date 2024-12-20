#pragma once

namespace crab
{
	enum class eLayoutFormat
	{
		None = 0,
		Float2,
		Float3,
		Float4
	};

	inline uint32 GetLayoutFormatSize(eLayoutFormat in_format)
	{
		switch (in_format)
		{
		case crab::eLayoutFormat::Float2: return 4 * 2;
		case crab::eLayoutFormat::Float3: return 4 * 3;
		case crab::eLayoutFormat::Float4: return 4 * 4;
		default: assert(false); return 0;
		}
	}

	struct LayoutElement
	{
		std::string		name;
		uint32			index;
		eLayoutFormat	format;
		//uint32		offset;
	};


	//===================================================
	//				      DirectX11
	//===================================================

	inline DXGI_FORMAT CvtLayoutFormatToDXGIFormat(eLayoutFormat in_format)
	{
		switch (in_format)
		{
		case eLayoutFormat::Float2: return DXGI_FORMAT_R32G32_FLOAT;
		case eLayoutFormat::Float3: return DXGI_FORMAT_R32G32B32_FLOAT; 
		case eLayoutFormat::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		default: assert(false); return DXGI_FORMAT_UNKNOWN;
		}
	}

}