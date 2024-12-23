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

		void							 SetViewport(float in_positionX, float in_positionY, float in_width, float in_height, float in_depthMin, float in_depthMax) override;
		void*							 GetTexture() const override;

		void							 Clear(const Vec4& in_clearColor, bool in_clearDepth, bool in_clearStencil) override;
		std::pair<float, float>			 GetSize() { return std::make_pair(m_viewport.Width, m_viewport.Height); };

		static Ref<DX11RenderTarget>	 CreateBySwapChain();

		::ComPtr<ID3D11RenderTargetView> GetRenderTargetView() const;
		::ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const;
		D3D11_VIEWPORT					 GetViewport() const;

	private:
		void	_create_(const RenderTargetProp& in_prop) override;

		::ComPtr<ID3D11RenderTargetView>	m_rtv;
		::ComPtr<ID3D11DepthStencilView>	m_dsv;
		::ComPtr<ID3D11ShaderResourceView>	m_srv;
		D3D11_VIEWPORT						m_viewport;
	};
}