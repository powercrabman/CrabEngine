#include "CrabEnginePch.h"
#include "DX11RenderTarget.h"
#include "ITexture.h"
#include "DX11Texture.h"

namespace crab
{
	void DX11RenderTarget::_create_(const RenderTargetProp& in_prop)
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();
		auto tex = static_pointer_cast<DX11Texture>(in_prop.baseTexture)->GetDX11Texture();

		DX_ASSERT(device->CreateRenderTargetView(tex.Get(), nullptr, &m_rtv), "Create rendertargetview fail.");
		DX_ASSERT(device->CreateDepthStencilView(tex.Get(), nullptr, &m_dsv), "Create depthstencilview fail.");
		DX_ASSERT(device->CreateShaderResourceView(tex.Get(), nullptr, &m_srv), "Create shaderresourceview fail.");

		SetViewport(
			in_prop.positionX,
			in_prop.positionY,
			in_prop.width,
			in_prop.height,
			in_prop.depthMin,
			in_prop.depthMax
		);
	}

	void DX11RenderTarget::SetViewport(float in_positionX, float in_positionY, float in_width, float in_height, float in_depthMin, float in_depthMax)
	{
		m_viewport = D3D11_VIEWPORT{
			.TopLeftX = in_positionX,
			.TopLeftY = in_positionY,
			.Width = in_width,
			.Height = in_height,
			.MinDepth = in_depthMin,
			.MaxDepth = in_depthMax
		};
	}

	Ref<DX11RenderTarget> DX11RenderTarget::CreateBySwapChain()
	{
		HR hr = {};
		ComPtr<IDXGISwapChain> sc = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetSwapchain();
		ComPtr<ID3D11Device> dv = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();
		Ref<DX11RenderTarget> rtv = MakeRef<DX11RenderTarget>();

		// RTV
		::ComPtr<ID3D11Texture2D> backBuffer;
		DX_ASSERT(sc->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())), "Failed to get Swap Chain back buffer");
		DX_ASSERT(dv->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv->m_rtv.GetAddressOf()), "Failed to create Render Target Veiw.");

		// SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		DX_ASSERT(dv->CreateShaderResourceView(backBuffer.Get(), &srvDesc, rtv->m_srv.GetAddressOf()), "Failed to create Shader Resource View for Swap Chain back buffer");

		// DSV
		DXGI_SWAP_CHAIN_DESC desc = {};
		DX_ASSERT(sc->GetDesc(&desc), "swapchain get desc fail.");

		const int width = desc.BufferDesc.Width;
		const int height = desc.BufferDesc.Height;

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		::ComPtr<ID3D11Texture2D> depthStencil;
		DX_ASSERT(dv->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()), "Failed to create Depth Stencil");
		DX_ASSERT(dv->CreateDepthStencilView(depthStencil.Get(), nullptr, rtv->m_dsv.GetAddressOf()), "Failed to create Depth Stencil");

		// viewport
		rtv->m_viewport.TopLeftX = 0;
		rtv->m_viewport.TopLeftY = 0;
		rtv->m_viewport.Width = width;
		rtv->m_viewport.Height = height;
		rtv->m_viewport.MinDepth = 0.0f;
		rtv->m_viewport.MaxDepth = 1.0f;

		return rtv;
	}

	void DX11RenderTarget::OnResize(const int in_width, const int in_height)
	{
		// TODO
	}
}
