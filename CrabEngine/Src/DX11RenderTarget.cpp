#include "CrabEnginePch.h"
#include "DX11RenderTarget.h"
#include "ITexture.h"
#include "DX11Texture.h"

namespace crab
{
	void DX11RenderTarget::_create_(const RenderTargetProp& in_prop)
	{
		m_clearColor = in_prop.clearColor;

		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		::ComPtr<ID3D11Texture2D> texture;

		// RTV
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = in_prop.width;
		texDesc.Height = in_prop.height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		DX_ASSERT(device->CreateTexture2D(&texDesc, nullptr, &texture), "Create Texture failed.");

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;
		DX_ASSERT(device->CreateRenderTargetView(texture.Get(), &rtvDesc, &m_rtv), "Create Render Target View failed.");

		// SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		DX_ASSERT(device->CreateShaderResourceView(texture.Get(), &srvDesc, &m_srv), "Create Shader Resource View failed.");

		m_viewport.Width = static_cast<float>(in_prop.width);
		m_viewport.Height = static_cast<float>(in_prop.height);
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
		m_viewport.MaxDepth = static_cast<float>(in_prop.maxDepth);
		m_viewport.MinDepth = static_cast<float>(in_prop.minDepth);
	}

	Ref<DX11RenderTarget> DX11RenderTarget::CreateBySwapChain()
	{
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

		// viewport
		rtv->m_viewport.TopLeftX = 0;
		rtv->m_viewport.TopLeftY = 0;
		rtv->m_viewport.Width = width;
		rtv->m_viewport.Height = height;
		rtv->m_viewport.MinDepth = 0.0f;
		rtv->m_viewport.MaxDepth = 1.0f;

		return rtv;
	}

	void DX11RenderTarget::Clear()
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
		context->ClearRenderTargetView(m_rtv.Get(), reinterpret_cast<float*>(&m_clearColor));
	}

	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
}
