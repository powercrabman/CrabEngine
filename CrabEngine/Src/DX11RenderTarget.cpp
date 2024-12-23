#include "CrabEnginePch.h"
#include "DX11RenderTarget.h"
#include "ITexture.h"
#include "DX11Texture.h"

namespace crab
{
	void DX11RenderTarget::_create_(const RenderTargetProp& in_prop)
	{
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

		// DSV
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width                = in_prop.width;
		depthStencilDesc.Height               = in_prop.height;
		depthStencilDesc.MipLevels            = 1;
		depthStencilDesc.ArraySize            = 1;
		depthStencilDesc.Format               = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Quality   = 0;
		depthStencilDesc.SampleDesc.Count     = 1;
		depthStencilDesc.Usage                = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags            = D3D11_BIND_DEPTH_STENCIL;

		::ComPtr<ID3D11Texture2D> depthStencil;
		DX_ASSERT(device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()), "Failed to create Depth Stencil");
		DX_ASSERT(device->CreateDepthStencilView(depthStencil.Get(), nullptr, m_dsv.GetAddressOf()), "Failed to create Depth Stencil");

		// SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		DX_ASSERT(device->CreateShaderResourceView(texture.Get(), &srvDesc, &m_srv), "Create Shader Resource View failed.");


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
			in_positionX,
			in_positionY,
			in_width,
			in_height,
			in_depthMin,
			in_depthMax
		};
	}

	void* DX11RenderTarget::GetTexture() const
	{
		return m_srv.Get();
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

	ComPtr<ID3D11RenderTargetView> DX11RenderTarget::GetRenderTargetView() const
	{
		return m_rtv;
	}

	ComPtr<ID3D11DepthStencilView> DX11RenderTarget::GetDepthStencilView() const
	{
		return m_dsv;
	}

	D3D11_VIEWPORT DX11RenderTarget::GetViewport() const
	{
		return m_viewport;
	}

	void DX11RenderTarget::Clear(const Vec4& in_clearColor, bool in_clearDepth, bool in_clearStencil)
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		context->ClearRenderTargetView(m_rtv.Get(), &in_clearColor.x);

		UINT flag = 0;
		flag |= in_clearDepth ? D3D11_CLEAR_DEPTH : 0;
		flag |= in_clearStencil ? D3D11_CLEAR_STENCIL : 0;
		context->ClearDepthStencilView(m_dsv.Get(), flag, 1.f, 0);
	}

}
