#pragma once
#include "LayoutElement.h"

namespace crab
{
	enum class eUnorderedAccessViewAccessType
	{
		None, // default setting
		Read
	};

	union UnorderedAccessViewClearType
	{
		UnorderedAccessViewClearType() {}
		void SetFloats(const std::array<float, 4>& in_values) { clearValueFloat = in_values; }
		void SetUInts(const std::array<uint32, 4>& in_values) { clearValueUInt = in_values; }

		std::array<float, 4>	clearValueFloat;
		std::array<uint32, 4>	clearValueUInt;
	};

	enum class eUnorderedAccessViewDataFormat
	{
		Float, UInt
	};

	struct UnorderedAccessViewProp
	{
		uint32 width;
		uint32 height;

		eUnorderedAccessViewDataFormat	format;
		eUnorderedAccessViewAccessType	accessType;
		UnorderedAccessViewClearType	clearValue;		
	};

	class IUnorderedAccessView
	{
	public:
				 IUnorderedAccessView() = default;
		virtual ~IUnorderedAccessView() = default;

		virtual void						Clear() = 0;
		virtual void						SetClearValue(const UnorderedAccessViewClearType& in_value) = 0;
		virtual std::pair<uint32, uint32>	GetSize() const = 0;
		virtual void						GetPixel(uint32 in_x, uint32 in_y, void* in_out_data) = 0;
		virtual void*						GetNativeView() const = 0;

		static Ref<IUnorderedAccessView>	Create(const UnorderedAccessViewProp& in_prop);

	private:
		virtual void _create_(const UnorderedAccessViewProp& in_prop) = 0;
	};
}

