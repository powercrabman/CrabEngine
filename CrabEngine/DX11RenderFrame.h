#pragma once
#include "DX11RenderTarget.h"
#include "IDepthStencil.h"
#include "IRenderFrame.h"

namespace crab
{
	class DX11RenderFrame : public IRenderFrame
	{
	public:
		void Clear(bool in_clearDepth, bool in_clearStencil) override;
		void Bind() override;
		void UnBind() override;

		uint32						GetRenderTargetCount() override { return m_renderTargets.size(); };
		Ref<IRenderTarget>			GetRenderTarget(uint32 in_slot) override { return m_renderTargets[in_slot]; }
		Ref<IUnorderedAccessView>	GetUnorderedResourceView(uint32 in_slot) override { return m_unorderedAccessView[in_slot]; }

	private:
		void _create_(const RenderFrameProp& in_prop) override;


		std::vector<Ref<IRenderTarget>>			m_renderTargets;
		std::vector<Ref<IUnorderedAccessView>>	m_unorderedAccessView;
		Ref<IDepthStencil>						m_depthStencil;


		std::vector<ID3D11RenderTargetView*>	m_dxRTV;
		std::vector<ID3D11UnorderedAccessView*>	m_dxUAV;
		std::vector<D3D11_VIEWPORT>				m_dxViewport;
	};
}


