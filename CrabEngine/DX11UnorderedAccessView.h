#pragma once
#include "IUnorderedAccessView.h"

namespace crab
{
	class DX11UnorderedAccessView : public IUnorderedAccessView
	{
	public:
		DX11UnorderedAccessView() = default;
		~DX11UnorderedAccessView() override = default;

		void						Clear() override;
		void						SetClearValue(const UnorderedAccessViewClearType& in_value) override { m_clearValue = in_value; }

		std::pair<uint32, uint32>	GetSize() const override { return std::make_pair(m_width, m_height); }
		void*						GetNativeView() const override { return m_uav.Get(); }
		void						GetPixel(uint32 in_x, uint32 in_y, void* in_out_data) override;

	private:
		void _create_(const UnorderedAccessViewProp& in_prop) override;
		DXGI_FORMAT _cvt_format_(eUnorderedAccessViewDataFormat in_format);
		uint32		_get_size_(eUnorderedAccessViewDataFormat in_format);

		::ComPtr<ID3D11UnorderedAccessView> m_uav;
		::ComPtr<ID3D11Texture2D>			m_stagingTex;

		eUnorderedAccessViewAccessType		m_accessType;
		eUnorderedAccessViewDataFormat		m_format;
		UnorderedAccessViewClearType		m_clearValue;
		uint32								m_width;
		uint32								m_height;
	};


}
