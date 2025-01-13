#pragma once
#include "IRenderTarget.h"

namespace crab
{
	class ITexture;

	class DX11RenderTarget : public IRenderTarget
	{
	public:
		DX11RenderTarget() = default;
		~DX11RenderTarget() override = default;

		void					Clear() override;
		void					SetClearColor(const Vec4& in_color) override { m_clearColor = in_color; }

		void*					GetNativeRenderTarget() override { return m_rtv.Get(); }
		void*					GetViewport() override { return &m_viewport; }
		void*					GetTexture() const override { return m_srv.Get(); }
		std::pair<float, float>	GetSize() const override { return std::make_pair(m_viewport.Width, m_viewport.Height); };
		float					GetAspect() const override { return m_viewport.Width / m_viewport.Height; }

		::ComPtr<ID3D11RenderTargetView>	GetRenderTargetView() const { return m_rtv; }
		static Ref<DX11RenderTarget>		CreateBySwapChain();

	private:
		void	_create_(const RenderTargetProp& in_prop) override;

	private:
		::ComPtr<ID3D11RenderTargetView>	m_rtv;
		::ComPtr<ID3D11ShaderResourceView>	m_srv;
		D3D11_VIEWPORT						m_viewport;
		Vec4								m_clearColor;
	};
}