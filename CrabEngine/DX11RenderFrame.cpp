#include "CrabEnginePch.h"
#include "DX11RenderFrame.h"

#include "IUnorderedAccessView.h"

namespace crab
{
	void DX11RenderFrame::Clear(bool in_clearDepth, bool in_clearStencil)
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		for (const auto& rt : m_renderTargets) { rt->Clear(); }
		for (const auto& uav : m_unorderedAccessView) { uav->Clear(); }
		m_depthStencil->Clear(in_clearDepth, in_clearStencil);
	}

	void DX11RenderFrame::Bind()
	{
		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		if (m_dxUAV.size())
		{
			context->OMSetRenderTargetsAndUnorderedAccessViews(
				m_dxRTV.size(),
				m_dxRTV.data(),
				static_cast<ID3D11DepthStencilView*>(m_depthStencil->GetNativeDepthStencil()),
				m_dxRTV.size(),
				m_dxUAV.size(),
				m_dxUAV.data(),
				nullptr
			);
		}
		else
		{
			context->OMSetRenderTargets(
				m_dxRTV.size(),
				m_dxRTV.data(),
				static_cast<ID3D11DepthStencilView*>(m_depthStencil->GetNativeDepthStencil())
			);
		}

		context->RSSetViewports(m_dxViewport.size(), m_dxViewport.data());
	}

	void DX11RenderFrame::UnBind()
	{

		auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();

		if (m_dxUAV.size())
		{
			context->OMSetRenderTargetsAndUnorderedAccessViews(0, nullptr, nullptr, 0, 0, nullptr, nullptr);
		}
		else
		{
			context->OMSetRenderTargets(0, nullptr, nullptr);
		}

		context->RSSetViewports(0, nullptr);
	}

	void DX11RenderFrame::_create_(const RenderFrameProp& in_prop)
	{
		m_renderTargets = in_prop.renderTargets;
		m_depthStencil = in_prop.depthStencil;
		m_unorderedAccessView = in_prop.unorderedAccessViews;

		m_dxRTV.reserve(m_renderTargets.size());
		m_dxViewport.reserve(m_renderTargets.size());
		for (const auto& rt : m_renderTargets)
		{
			m_dxRTV.push_back(static_cast<ID3D11RenderTargetView*>(rt->GetNativeRenderTarget()));
			m_dxViewport.push_back(*static_cast<D3D11_VIEWPORT*>(rt->GetViewport()));
		}

		m_dxUAV.reserve(m_unorderedAccessView.size());
		for (const auto& uav : m_unorderedAccessView)
		{
			m_dxUAV.push_back(static_cast<ID3D11UnorderedAccessView*>(uav->GetNativeView()));
		}
	}
}
