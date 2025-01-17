#include "CrabEnginePch.h"
#include "DX11DepthStencil.h"

namespace crab
{
	void DX11DepthStencil::Clear(bool in_clearDepth, bool in_clearStencil)
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		uint32 clearFlag = 0;
		clearFlag        |= in_clearDepth ? D3D11_CLEAR_DEPTH : 0;
		clearFlag        |= in_clearStencil ? D3D11_CLEAR_STENCIL : 0;
		context->ClearDepthStencilView(m_dsv.Get(), clearFlag, 1.f, 0);
	}

	void DX11DepthStencil::create_depthstencil(uint32 in_width, uint32 in_height)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = in_width;
		depthStencilDesc.Height = in_height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		::ComPtr<ID3D11Texture2D> depthStencil;
		DX_ASSERT(device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()), "Failed to create Depth Stencil");
		DX_ASSERT(device->CreateDepthStencilView(depthStencil.Get(), nullptr, m_dsv.GetAddressOf()), "Failed to create Depth Stencil");
	}
}
