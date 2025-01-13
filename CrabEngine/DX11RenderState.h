#pragma once
#include "IRenderState.h"

namespace crab
{
	class DX11RenderState : public IRenderState
	{
	public:
		DX11RenderState();
		~DX11RenderState() = default;

		void BindBlendState(eBlendState in_state) override
		{
			static const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; 
			auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
			context->OMSetBlendState(m_blendStates[(uint32)in_state].Get(), blendFactor, 0xFFFFFFFF);
		}

		void BindDepthStencilState(eDepthStencilState in_state) override
		{
			auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
			context->OMSetDepthStencilState(m_depthStencilStates[(uint32)in_state].Get(), 0);
		}

		void BindRasterizerState(eRasterizerState in_state) override
		{
			auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
			context->RSSetState(m_rasterizerStates[(uint32)in_state].Get());
		}

		void BindSamplerState(eSamplerState in_state, uint32 in_slot) override
		{
			auto context = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetContext();
			context->PSSetSamplers(in_slot, 1, m_samplerStates[(uint32)in_state].GetAddressOf());
		}

	private:
		std::array<::ComPtr<ID3D11BlendState>, (uint32)eBlendState::Count>					m_blendStates;
		std::array<::ComPtr<ID3D11DepthStencilState>, (uint32)eDepthStencilState::Count>	m_depthStencilStates;
		std::array<::ComPtr<ID3D11RasterizerState>, (uint32)eRasterizerState::Count>		m_rasterizerStates;
		std::array<::ComPtr<ID3D11SamplerState>, (uint32)eSamplerState::Count>				m_samplerStates;
	};
}

