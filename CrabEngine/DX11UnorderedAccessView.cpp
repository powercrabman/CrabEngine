#include "CrabEnginePch.h"
#include "DX11UnorderedAccessView.h"

namespace crab
{
	void DX11UnorderedAccessView::Clear()
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		if (m_format == eUnorderedAccessViewDataFormat::UInt)
		{
			context->ClearUnorderedAccessViewUint(m_uav.Get(), m_clearValue.clearValueUInt.data());
		}
		else
		{
			context->ClearUnorderedAccessViewFloat(m_uav.Get(), m_clearValue.clearValueFloat.data());
		}
	}

	void DX11UnorderedAccessView::GetPixel(uint32 in_x, uint32 in_y, void* in_out_data)
	{
		assert(m_accessType == eUnorderedAccessViewAccessType::Read);
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		::ComPtr<ID3D11Resource> uavRes;
		m_uav->GetResource(uavRes.GetAddressOf());

		if (!math::IsBetweenWith(in_x, 0u, static_cast<uint32>(m_width) - 1)) return;
		if (!math::IsBetweenWith(in_y, 0u, static_cast<uint32>(m_height) - 1)) return;

		in_x = std::clamp(in_x, 0u, static_cast<uint32>(m_width - 1));
		in_y = std::clamp(in_y, 0u, static_cast<uint32>(m_height - 1));

		D3D11_BOX sourceRegion = {};
		sourceRegion.left = in_x;
		sourceRegion.right = in_x + 1;
		sourceRegion.top = in_y;
		sourceRegion.bottom = in_y + 1;
		sourceRegion.front = 0;
		sourceRegion.back = 1;

		context->CopySubresourceRegion(m_stagingTex.Get(), 0, 0, 0, 0, uavRes.Get(), 0, &sourceRegion);

		D3D11_MAPPED_SUBRESOURCE mapRes;
		DX_ASSERT(context->Map(m_stagingTex.Get(), 0, D3D11_MAP_READ, 0, &mapRes), "map fail.");

		uint32 pixelSize = _get_size_(m_format);
		uint8* pixel = static_cast<uint8*>(mapRes.pData);
		memcpy(in_out_data, pixel, pixelSize);

		context->Unmap(m_stagingTex.Get(), 0);
	}

	void DX11UnorderedAccessView::_create_(const UnorderedAccessViewProp& in_prop)
	{
		m_accessType = in_prop.accessType;
		m_width = in_prop.width;
		m_height = in_prop.height;
		m_format = in_prop.format;
		m_clearValue = in_prop.clearValue;
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = in_prop.width;
		texDesc.Height = in_prop.height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = _cvt_format_(m_format);
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
		texDesc.CPUAccessFlags = 0;

		::ComPtr<ID3D11Texture2D> tex = nullptr;
		DX_ASSERT(device->CreateTexture2D(&texDesc, nullptr, &tex), "creation fail.");
		DX_ASSERT(device->CreateUnorderedAccessView(tex.Get(), nullptr, m_uav.GetAddressOf()), "creation fail.");

		if (in_prop.accessType == eUnorderedAccessViewAccessType::Read)
		{
			D3D11_TEXTURE2D_DESC texDesc = {};
			texDesc.Width = in_prop.width;
			texDesc.Height = in_prop.height;
			texDesc.Format = _cvt_format_(m_format);
			texDesc.Usage = D3D11_USAGE_STAGING;
			texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			texDesc.MipLevels = 1;
			texDesc.ArraySize = 1;
			texDesc.SampleDesc.Count = 1;
			DX_ASSERT(device->CreateTexture2D(&texDesc, nullptr, m_stagingTex.GetAddressOf()), "creation fail.");
		}

	}

	DXGI_FORMAT DX11UnorderedAccessView::_cvt_format_(eUnorderedAccessViewDataFormat in_format)
	{
		switch (in_format)
		{
		case eUnorderedAccessViewDataFormat::Float:	return DXGI_FORMAT_R8G8B8A8_UNORM;
		case eUnorderedAccessViewDataFormat::UInt:	return DXGI_FORMAT_R32_UINT;
		default: assert(false);						return DXGI_FORMAT_UNKNOWN;
		}
	}

	uint32 DX11UnorderedAccessView::_get_size_(eUnorderedAccessViewDataFormat in_format)
	{
		switch (in_format)
		{
		case eUnorderedAccessViewDataFormat::Float:	return 4;
		case eUnorderedAccessViewDataFormat::UInt:	return 4;
		default: assert(false);						return 0;
		}
	}
}
