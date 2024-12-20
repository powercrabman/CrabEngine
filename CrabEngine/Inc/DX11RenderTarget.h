#pragma once
#include "IRenderTarget.h"

namespace crab
{
	class ITexture;

	class DX11RenderTarget : public IRenderTarget
	{
	public:
		DX11RenderTarget() = default;
		~DX11RenderTarget() = default;

		void	SetViewport(float in_positionX, float in_positionY, float in_width, float in_height, float in_depthMin, float in_depthMax) override;
		void*	GetTexture() const override { return m_srv.Get(); }


		void	Clear(const Vec4& in_clearColor, bool in_clearDepth, bool in_clearStencil) override;
		void	OnResize(const int in_width, const int in_height) override;

		static Ref<DX11RenderTarget> CreateBySwapChain();

		::ComPtr<ID3D11RenderTargetView> GetRenderTargetView() const { return m_rtv; }
		::ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const { return m_dsv; }
		D3D11_VIEWPORT					 GetViewport() const { return m_viewport; }

	private:
		void	_create_(const RenderTargetProp& in_prop) override;

		::ComPtr<ID3D11RenderTargetView>	m_rtv;
		::ComPtr<ID3D11DepthStencilView>	m_dsv;
		::ComPtr<ID3D11ShaderResourceView>	m_srv;
		D3D11_VIEWPORT						m_viewport;
	};

	//===================================================
	//                      Inline
	//===================================================

	inline void DX11RenderTarget::Clear(const Vec4& in_clearColor, bool in_clearDepth, bool in_clearStencil)
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		context->ClearRenderTargetView(m_rtv.Get(), &in_clearColor.x);

		UINT flag = 0;
		flag |= in_clearDepth ? D3D11_CLEAR_DEPTH : 0;
		flag |= in_clearStencil ? D3D11_CLEAR_STENCIL : 0;
		context->ClearDepthStencilView(m_dsv.Get(), flag, 1.f, 0);
	}
}